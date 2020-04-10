/*
* MIT License
*
* Copyright (c) 2019 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

namespace Curse
{

    namespace Constants
    {

        template<>
        constexpr float Pi()
        {
            return static_cast<float>(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899L);
        }

        template<>
        constexpr double Pi()
        {
            return static_cast<double>(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899);
        }

        template<>
        constexpr long double Pi()
        {
            return static_cast<long double>(3.14159265358979323846264338327950288419716939937510582097494459230781640628620899L);
        }

    }

    template<typename T>
    constexpr T Power2(const size_t exponent)
    {
        return exponent == 0 ? static_cast<T>(1) : static_cast<T>(2) * Power2<T>(exponent-1);
    }

}