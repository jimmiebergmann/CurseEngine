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

#ifndef CURSE_CORE_MATH_MATRIX_HPP
#define CURSE_CORE_MATH_MATRIX_HPP

#include "Curse/Math.hpp"
#include "Curse/Math/Vector.hpp"

namespace Curse
{

    /**
    * @brief Linear algebra class for matrix.
    */
    template<size_t _Rows, size_t _Columns, typename T>
    class Matrix
    {

    public:

        static constexpr size_t Rows = _Rows;
        static constexpr size_t Columns = _Columns;
        static constexpr size_t Components = Rows * Columns;
        using Type = T;

        /**
        * @brief Constructor.
        *        Elements are uninitialized.
        */
        Matrix();

        union
        {
            T e[Components]; //< Elements of matrix.
            Vector<Columns, T> row[Rows];  //< Rows of matrix.
        };

    };

    /**
    * @brief Linear algebra class for matrix.
    */
    template<typename T>
    class Matrix<4, 4, T>
    {

    public:

        static constexpr size_t Rows = 4;
        static constexpr size_t Columns = 4;
        static constexpr size_t Components = Rows * Columns;
        using Type = T;

        /**
        * @brief Creates a perspective projection matrix.
        */
        static Matrix<4, 4, T> Perspective(const T fov, const T aspect, const T zNear, const T zFar);


        /**
        * @brief Creates an orthographic projection matrix.
        */
        static Matrix<4, 4, T> Orthographic(const T left, const T right, const T bottom, const T top,
                                            const T zNear, const T zFar);

        /**
        * @brief Constructor.
        *        Elements are uninitialized.
        */
        Matrix();

        /**
        * @brief Constructing and initializing all elements.
        */
        Matrix(const T e1,  const T e2,  const T e3,  const T e4,
               const T e5,  const T e6,  const T e7,  const T e8,
               const T e9,  const T e10, const T e11, const T e12,
               const T e13, const T e14, const T e15, const T e16);

        /**
        * @brief Constructing and initializing all elements.
        */
        Matrix(const Vector4<T>& row1, const Vector4<T>& row2, const Vector4<T>& row3, const Vector4<T>& row4);

        union
        {        
            T e[Components];    //< Elements of matrix.
            Vector4<T> row[4];  //< Rows of matrix.
        };

    };

    template<typename T>
    using Matrix4x4 = Matrix<4, 4, T>;
    using Matrix4x4i32 = Matrix4x4<int32_t>;
    using Matrix4x4i64 = Matrix4x4<int64_t>;
    using Matrix4x4f32 = Matrix4x4<float>;
    using Matrix4x4f64 = Matrix4x4<double>;

}

#include "Matrix.inl"

#endif