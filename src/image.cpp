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

#include "image.hpp"

#include <lodepng.h>

Image::Image(const unsigned int width, const unsigned int height) : _width(width), _height(height), _data((width * height + 7) / 8, 0) {}

void Image::fillRectangle(const unsigned int x, const unsigned int y, const unsigned int w, const unsigned int h) {
    for (unsigned int i = 0; i < w; i++) {
        for (unsigned int j = 0; j < h; j++) {
            fillPixel(x + i, y + j);
        }
    }
}

inline void Image::fillPixel(const unsigned int x, const unsigned int y) {
    const unsigned int index = y * _width + x;
    _data[index / 8] |= 1 << (7 - index % 8);
}

void Image::writeToFile(const std::string& fileName) const {
    lodepng::encode(fileName, _data, _width, _height, LCT_GREY, 1);
}
