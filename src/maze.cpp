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

#include "maze.hpp"

#include <cmath>
#include <stdexcept>

#include "random_queue.hpp"
#include "vector_util.hpp"

Maze::Maze(const unsigned int width, const unsigned int height) : _width(width), _height(height), _size(width * height) {
    _points.reserve(_size);
    for (int i = 0; i < _size; i++) {
        _points.emplace_back(*this, i);
    }
}

void Maze::connectAll(std::mt19937 &generator, const double errorFactor) {
    if (errorFactor < 0 || errorFactor > 1) {
        throw std::range_error("Error factor must be between 0 and 1");
    }

    for (unsigned int i = 0; i < _size; i++) {
        _points[i].shuffleDirectionCombination(generator);
    }

    const unsigned int max = _size - 1;
    unsigned int connections = 0;
    RandomQueue queue(toPointers(_points), generator);

    while (connections != max) {
        if (queue.next()->tryConnect()) {
            connections++;
        }
    }

    const unsigned int errors = std::lround((_size - _width - _height + 1) * errorFactor);
    if (errors == 0) {
        return;
    }

    for (unsigned int i = 0; i < _size; i++) {
        _points[i].resetDirectionIndex();
    }

    connections = 0;
    queue.reset();

    while (connections != errors) {
        if (queue.next()->forceConnect()) {
            connections++;
        }
    }
}

Image Maze::generateImage(const unsigned int pathSize, const unsigned int wallSize) const {
    Image image(_width * pathSize + (_width + 1) * wallSize, _height * pathSize + (_height + 1) * wallSize);

    unsigned int pos = 0;
    unsigned int imgY = wallSize;
    for (int y = 0; y < _height; y++) {
        unsigned int imgX = wallSize;
        for (int x = 0; x < _width; x++) {
            const Point p = _points[pos];

            image.fillRectangle(imgX, imgY, pathSize, pathSize);
            if (p._connectedRight) {
                image.fillRectangle(imgX + pathSize, imgY, wallSize, pathSize);
            }
            if (p._connectedDown) {
                image.fillRectangle(imgX, imgY + pathSize, pathSize, wallSize);
            }

            pos++;
            imgX += pathSize + wallSize;
        }
        imgY += pathSize + wallSize;
    }

    return image;
}

Point::Point(Maze &maze, const unsigned int position) : _maze(maze), _position(position) {}

bool Point::append(Point& other) {
    Point *a = top(), *b = other.top();
    if (a == b) {
        return false;
    }
    a->_parent = b;
    return true;
}

Point* Point::top() {
    if (_parent == nullptr) {
        return this;
    }
    Point* t = _parent->top();
    _parent = t;
    return t;
}

void Point::shuffleDirectionCombination(std::mt19937 &generator) {
    _directions = &randomDirectionCombination(generator);
    resetDirectionIndex();
}

void Point::resetDirectionIndex() {
    _directionIndex = -1;
}

bool Point::tryConnect() {
    while (_directionIndex < 3) {
        _directionIndex++;
        if (tryConnect((*_directions)[_directionIndex])) {
            return true;
        }
    }
    return false;
}

bool Point::tryConnect(const Direction direction) {
    unsigned int rel;
    switch (direction) {
        case UP:
            rel = _position - _maze._width;
            if (rel > _position) { // underflow
                return false;
            }

        {
            Point& p = _maze._points[rel];
            if (!append(p)) {
                return false;
            }

            p._connectedDown = true;
        }
            return true;
        case DOWN:
            rel = _position + _maze._width;
            if (rel >= _maze._size) {
                return false;
            }

            if (!append(_maze._points[rel])) {
                return false;
            }

            _connectedDown = true;
            return true;
        case LEFT:
            if (_position % _maze._width == 0) {
                return false;
            }

        {
            Point& p = _maze._points[_position - 1];
            if (!append(p)) {
                return false;
            }

            p._connectedRight = true;
        }
            return true;
        case RIGHT:
            rel = _position + 1;
            if (rel % _maze._width == 0) {
                return false;
            }

            if (!append(_maze._points[rel])) {
                return false;
            }

            _connectedRight = true;
            return true;
    }
    throw std::invalid_argument("Invalid direction");
}

bool Point::forceConnect() {
    while (_directionIndex < 3) {
        _directionIndex++;
        if (forceConnect((*_directions)[_directionIndex])) {
            return true;
        }
    }
    return false;
}

bool Point::forceConnect(const Direction direction) {
    unsigned int rel;
    switch (direction) {
        case UP:
            rel = _position - _maze._width;
            if (rel > _position) { // underflow
                return false;
            }

        {
            Point& p = _maze._points[rel];
            if (p._connectedDown) {
                return false;
            }

            p._connectedDown = true;
        }
            return true;
        case DOWN:
            rel = _position + _maze._width;
            if (rel >= _maze._size) {
                return false;
            }

            if (_connectedDown) {
                return false;
            }

            _connectedDown = true;
            return true;
        case LEFT:
            if (_position % _maze._width == 0) {
                return false;
            }

        {
            Point& p = _maze._points[_position - 1];
            if (p._connectedRight) {
                return false;
            }

            p._connectedRight = true;
        }
            return true;
        case RIGHT:
            rel = _position + 1;
            if (rel % _maze._width == 0) {
                return false;
            }

            if (_connectedRight) {
                return false;
            }

            _connectedRight = true;
            return true;
    }
    throw std::invalid_argument("Invalid direction");
}
