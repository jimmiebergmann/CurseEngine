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

#ifndef CURSE_CORE_GUI_GUICANVAS_HPP
#define CURSE_CORE_GUI_GUICANVAS_HPP

#include "Curse/Types.hpp"
#include "Curse/Gui/Control/GuiPlaneControl.hpp"

namespace Curse
{

    // Forward declarations.
    class Renderer;


    namespace Gui
    {

        /**
        * @brief GUI canvas class.
        *        Managing root controlers and device inputs.
        */
        class CURSE_API Canvas
        {

        public:

            /**
            * @brief Constructor.
            */
            Canvas(Renderer& renderer, const Vector2f32& size, const Vector2f32& position = {0.0f, 0.0f});

            /**
            * @brief Destructor.
            */
            ~Canvas();

            /**
            * @brief Update the canvas.
            */
            void Update();

            /**
            * @brief Render the canvas.
            */
            void Draw();

            /**
            * @brief Get relative position to origin.
            */
            Vector2f32 GetPosiion() const;

            /**
            * @brief Get canvas size.
            */
            Vector2f32 GetSize() const;

            /**
            * @brief Get underlying plane control.
            */
            Plane& GetPlane();

            /**
            * @brief Get underlying plane control.
            */
            const Plane& GetPlane() const;

            /**
            * @brief Set relative position to origin.
            */
            void GetPosiion(const Vector2f32& position);

            /**
            * @brief Set canvas size.
            */
            void SetSize(const Vector2f32& size);

        private:

            Renderer * m_renderer;
            Vector2f32 m_position;
            Vector2f32 m_size;
            Plane m_plane;

        };

    }

}

#endif
