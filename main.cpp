/*
* Copyright (c) 2025 Hugo Dupanloup (Yeregorix)
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

#include <iostream>
#include <string>
#include <ctime>

#include "chrono.hpp"
#include "image.hpp"
#include "lodepng.h"
#include "maze.hpp"
#include "string_util.hpp"

using namespace std;

string readString() {
    string line;
    getline(cin, line);
    trim(line);
    return line;
}

string readString(const string& defaultValue) {
    const string line = readString();
    return line.empty() ? defaultValue : line;
}

int readInt(const int defaultValue) {
    const string line = readString();
    return line.empty() ? defaultValue : stoi(line);
}

unsigned int readUInt(const unsigned int defaultValue) {
    const string line = readString();
    return line.empty() ? defaultValue : stoi(line);
}

double readDouble(const double defaultValue) {
    const string line = readString();
    return line.empty() ? defaultValue : stod(line);
}

int main()
{
    cout << "Maze width? ";
    const int width = readInt(0);
    if (width <= 0) {
        throw domain_error("Maze width must be strictly positive");
    }

    cout << "Maze height (" << width << ")? ";
    const int height = readInt(width);
    if (height <= 0) {
        throw domain_error("Maze height must be strictly positive");
    }

    cout << "Wall size (1)? ";
    const int wallSize = readInt(1);
    if (wallSize <= 0) {
        throw domain_error("White pixels must be strictly positive");
    }

    cout << "Path size (" << (wallSize * 2) << ")? ";
    const int pathSize = readInt(wallSize * 2);
    if (pathSize <= 0) {
        throw domain_error("Black pixels must be strictly positive");
    }

    cout << "Seed (time)? ";
    const unsigned int seed = readUInt(time(nullptr));

    cout << "File (./output.png)? ";
    const string fileName = readString("./output.png");

    cout << "Generating maze ... (" << width << "x" << height << ", seed:" << seed << ")" << endl;
    Chrono chrono;

    srand(seed);

    Maze maze(width, height);
    maze.connectAll(0);

    chrono.done();

    cout << "Generating image ... (" << pathSize << ":" << wallSize << ")" << endl;
    chrono.restart();

    const Image image = maze.generateImage(pathSize, wallSize);

    chrono.done();

    cout << "Writing to file ... (" << fileName << ")" << endl;
    chrono.restart();

    image.writeToFile(fileName);

    chrono.done();

    return 0;
}
