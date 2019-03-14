/* test.h
 *
 * Copyright (C) 2012-2013  Alexander Kojevnikov <alexander@kojevnikov.com>
 *
 * Spek is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spek is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spek.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TEST_H_
#define TEST_H_

#include <cmath>
#include <functional>
#include <iostream>
#include <string>

void run(std::function<void ()> func, const std::string& message);

extern int g_total;
extern int g_passes;

template<class T> bool equal(const T& a, const T& b)
{
    return a == b;
}

template<> inline bool equal<double>(const double& a, const double& b)
{
    return std::abs(a - b) < 1e-8;
}

template<class T> void test(const std::string& message, const T& expected, const T& actual)
{
    g_total++;
    if (equal(expected, actual)) {
        g_passes++;
    } else {
        std::cerr << "FAIL: " << message;
        std::cerr << ", expected: " << expected;
        std::cerr << ", actual: " << actual << std::endl;
    }
}

void test_audio_info();
void test_utils();

#endif
