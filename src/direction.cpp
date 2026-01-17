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

#include "direction.hpp"

#include <array>

typedef std::array<DirectionCombination, 24> DirectionCombinationSet;

void computeCombinations(DirectionCombinationSet& combinations) {
    int i = 0;
    for (const Direction& d1 : DIRECTIONS) {
        for (const Direction& d2 : DIRECTIONS) {
            if (d2 == d1) {
                continue;
            }
            for (const Direction& d3 : DIRECTIONS) {
                if (d3 == d1 || d3 == d2) {
                    continue;
                }
                for (const Direction& d4 : DIRECTIONS) {
                    if (d4 == d1 || d4 == d2 || d4 == d3) {
                        continue;
                    }
                    combinations[i++] = {d1, d2, d3, d4};
                }
            }
        }
    }
}

DirectionCombinationSet combinationsCache;
bool combinationsCacheInitialized = false;

DirectionCombination& randomDirectionCombination(std::mt19937 &generator) {
    if (!combinationsCacheInitialized) {
        computeCombinations(combinationsCache);
        combinationsCacheInitialized = true;
    }
    static std::uniform_int_distribution distribution(0, 23);
    return combinationsCache[distribution(generator)];
}
