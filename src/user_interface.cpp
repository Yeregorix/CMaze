/*
* Copyright (c) 2026 Hugo Dupanloup (Yeregorix)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include "user_interface.hpp"

#include <iostream>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <random>

#include "chrono.hpp"
#include "maze.hpp"

UserInterface::UserInterface(QWidget *parent) : QWidget(parent) {
    setFixedWidth(400);

    _seed = new QSpinBox();
    _width = new QSpinBox(), _height = new QSpinBox();
    _error = new QDoubleSpinBox();
    _pathSize = new QSpinBox(), _wallSize = new QSpinBox();

    _seed->setMinimum(INT_MIN);
    _seed->setMaximum(INT_MAX);
    _seed->setValue(0);

    _width->setMinimum(1);
    _width->setMaximum(10000);
    _width->setValue(30);

    _height->setMinimum(1);
    _height->setMaximum(10000);
    _height->setValue(30);

    _error->setMinimum(0);
    _error->setMaximum(1);
    _error->setValue(0);
    _error->setSingleStep(0.1);

    _pathSize->setMinimum(1);
    _pathSize->setMaximum(100);
    _pathSize->setValue(2);

    _wallSize->setMinimum(1);
    _wallSize->setMaximum(100);
    _wallSize->setValue(1);

    auto *randomSeedButton = new QPushButton("Random");
    auto *generateButton = new QPushButton("Generate");

    connect(randomSeedButton, SIGNAL (clicked(bool)), this, SLOT (randomSeed()));
    connect(generateButton, SIGNAL (clicked(bool)), this, SLOT (generate()));

    auto *layout = new QGridLayout(this);

    layout->addWidget(new QLabel("Seed:"), 0, 0);
    layout->addWidget(_seed, 0, 1);
    layout->addWidget(randomSeedButton, 0, 2);

    layout->addWidget(new QLabel("Size:"), 1, 0);
    layout->addWidget(_width, 1, 1);
    layout->addWidget(_height, 1, 2);

    layout->addWidget(new QLabel("Error:"), 2, 0);
    layout->addWidget(_error, 2, 1);

    layout->addWidget(new QLabel("Pixels:"), 3, 0);
    layout->addWidget(_pathSize, 3, 1);
    layout->addWidget(_wallSize, 3, 2);

    layout->addWidget(generateButton, 4, 0, 1, 3);

    layout->setColumnStretch(0, 10);
    layout->setColumnStretch(1, 45);
    layout->setColumnStretch(2, 45);

    _fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    _fileDialog.setNameFilter("Image (*.png)");
    _fileDialog.setDirectory(QDir::homePath());
}

void UserInterface::randomSeed() const {
    static std::mt19937 generator(std::random_device{}());
    static std::uniform_int_distribution distribution(INT_MIN, INT_MAX);
    _seed->setValue(distribution(generator));
}

void UserInterface::generate() {
    if (_fileDialog.exec()) {
        const QString qFileName = _fileDialog.selectedFiles().first();
        _fileDialog.setDirectory(QFileInfo(qFileName).path());

        const std::string fileName = qFileName.toStdString();
        const int seed = _seed->value();
        const int width = _width->value(), height = _height->value();
        const double error = _error->value();
        const int pathSize = _pathSize->value(), wallSize = _wallSize->value();

        std::cout << "Generating maze ... (" << width << "x" << height << ", error:" << error << ", seed:" << seed << ")" << std::endl;
        Chrono chrono;

        std::mt19937 generator(seed);

        Maze maze(width, height);
        maze.connectAll(generator, error);

        chrono.done();

        std::cout << "Generating image ... (" << pathSize << ":" << wallSize << ")" << std::endl;
        chrono.restart();

        const Image image = maze.generateImage(pathSize, wallSize);

        chrono.done();

        std::cout << "Writing to file ... (" << fileName << ")" << std::endl;
        chrono.restart();

        image.writeToFile(fileName);

        chrono.done();
    }
}
