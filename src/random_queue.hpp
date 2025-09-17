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

#ifndef RANDOM_QUEUE_HPP
#define RANDOM_QUEUE_HPP

#include <cstdlib>
#include <vector>

template <typename T>
class RandomQueue {

    public:

    explicit RandomQueue(std::vector<T> values) : _values(values), _remainingSize(0) {};

    void reset() {
        _remainingSize = 0;
    }

    T next() {
        if (_remainingSize == 0) {
            _remainingSize = _values.size();
        }
        int index = rand() % _remainingSize;
        T value = _values[index];
        _remainingSize--;
        _values[index] = _values[_remainingSize];
        _values[_remainingSize] = value;
        return value;
    }

    private:

    std::vector<T> _values;
    int _remainingSize;
};

#endif //RANDOM_QUEUE_HPP
