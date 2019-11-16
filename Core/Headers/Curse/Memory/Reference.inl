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

    template<typename T>
    template<typename ... Args>
    inline Reference<T> Reference<T>::Create(Args ... args)
    {
        return { new Controller(new T(args...)) };
    }

    template<typename T>
    inline Reference<T>::Reference() :
        m_controller(nullptr)
    {
    }
 
    template<typename T>
    inline Reference<T>::Reference(const Reference& ref) :
        m_controller(nullptr)
    {
        auto old = m_controller;
        m_controller = ref.m_controller;
        if (m_controller && m_controller != old)
        {
            ++m_controller->m_counter;
        }
    }

    template<typename T>
    Reference<T>& Reference<T>::operator = (const Reference& ref)
    {
        auto old = m_controller;
        m_controller = ref.m_controller;
        if (m_controller && m_controller != old)
        {
            ++m_controller->m_counter;
        }
        return *this;
    }

    template<typename T>
    inline Reference<T>::Reference(Reference&& ref) :
        m_controller(nullptr)
    {
        auto old = m_controller;
        m_controller = ref.m_controller;
        ref.m_controller = nullptr;
        if (m_controller)
        {
            ref.m_controller = nullptr;
        
            if (m_controller == old)
            {
                size_t counter = m_controller->m_counter--;
                if (!counter && m_controller->m_object)
                {
                    delete m_controller;
                }
            }
        }
    }

    template<typename T>
    Reference<T>& Reference<T>::operator = (Reference&& ref)
    {
        auto old = m_controller;
        m_controller = ref.m_controller;
        ref.m_controller = nullptr;
        if (m_controller)
        {
            ref.m_controller = nullptr;
            
            if (m_controller == old)
            {
                size_t counter = m_controller->m_counter--;
                if (!counter && m_controller->m_object)
                {
                    delete m_controller;
                }
            }          
        }

        return *this;
    }

    template<typename T>
    T& Reference<T>::operator *() const
    {
        if (m_controller)
        {
            return *m_controller->m_object;
        }

        throw Exception("Accessing null ptr.");
    }

    template<typename T>
    T* Reference<T>::Get() const
    {
        return m_controller ? m_controller->m_object : nullptr;
    }

    template<typename T>
    inline Reference<T>::~Reference()
    {
        if (m_controller)
        {
            size_t counter = m_controller->m_counter--;
            if (!counter && m_controller->m_object)
            {
                delete m_controller;
            }
        }
    }

    template<typename T>
    inline size_t Reference<T>::GetUseCount() const
    {
        return m_controller ? m_controller->m_counter : 0;
    }

    template<typename T>
    inline Reference<T>::Reference(Controller* controlObject) :
        m_controller(controlObject)
    {
    }


    template<typename T>
    inline Reference<T>::Controller::Controller(T* object) :
        m_object(object),
        m_counter(1)
    {
    }

    template<typename T>
    inline Reference<T>::Controller::~Controller()
    {
        if (m_object)
        {
            delete m_object;
        }
    }

}