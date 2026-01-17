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

#include "worker.hpp"

#include <iostream>

#include "chrono.hpp"
#include "maze.hpp"

Worker::Worker(const WorkerParameters &parameters) : _parameters(parameters) {
    setAutoDelete(true);
}

void Worker::run() {
    const auto [seed, width, height, errorFactor, pathSize, wallSize, fileName] = _parameters;

    std::cout << "Generating maze ... (" << width << "x" << height << ", error:" << errorFactor << ", seed:" << seed << ")" << std::endl;
    Chrono chrono;

    emit message("Initializing ...");
    std::mt19937 generator(seed);
    Maze maze(width, height);
    maze._worker = this;

    emit message("Filling points ...");
    maze.fill();

    emit message("Connecting points ...");
    maze.connectAll(generator, errorFactor);

    chrono.done();

    if (!_cancelled) {
        std::cout << "Generating image ... (" << pathSize << ":" << wallSize << ")" << std::endl;
        chrono.restart();

        emit message("Generating image ...");
        const QBitmap image = maze.generateImage(pathSize, wallSize);

        chrono.done();

        if (!_cancelled) {
            std::cout << "Writing to file ... (" << fileName.toStdString() << ")" << std::endl;
            chrono.restart();

            emit message("Writing image ...");
            const bool writeResult = image.save(fileName, "PNG");

            chrono.done();
            std::cout << "Write " << (writeResult ? "succeeded" : "failed") << "." << std::endl;
        }
    }

    const auto taskResult = _cancelled ? "Task cancelled." : "Task completed.";
    std::cout << taskResult << std::endl;
    emit message(taskResult);

    emit finished();
}

bool Worker::isCancelled() const {
    return _cancelled;
}

void Worker::cancel() {
    _cancelled = true;
}
