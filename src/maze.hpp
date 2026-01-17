/*
* Copyright (c) 2025-2026 Hugo Dupanloup (Yeregorix)
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

#ifndef MAZE_HPP
#define MAZE_HPP

#include <QBitmap>
#include <vector>
#include <random>
#include "direction.hpp"

class Point;

class Maze {
    friend class Point;

    public:

    Maze(unsigned int width, unsigned int height);

    void connectAll(std::mt19937 &generator, double errorFactor);

    [[nodiscard]] QBitmap generateImage(int pathSize, int wallSize) const;

    private:

    unsigned int _width, _height, _size;
    std::vector<Point> _points{};
};

class Point {
    friend class Maze;

    public:

    Point(Maze& maze, unsigned int position);

    private:

    Maze& _maze;
    unsigned int _position;

    Point* _parent = nullptr;
    bool _connectedRight = false, _connectedDown = false;

    DirectionCombination const* _directions = &DIRECTIONS;
    short _directionIndex = 0;

    bool append(Point& other);

    Point* top();

    void shuffleDirectionCombination(std::mt19937 &generator);

    void resetDirectionIndex();

    bool tryConnect();

    bool tryConnect(Direction direction);

    bool forceConnect();

    bool forceConnect(Direction direction);
};

#endif //MAZE_HPP
