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


// Windows
#if defined( _WIN32 ) || defined( __WIN32__ ) || defined( _WIN64 ) || defined( __WIN64__ )
    #define CURSE_PLATFORM_WINDOWS
// Linux
#elif defined( linux ) || defined( __linux )
    #define CURSE_PLATFORM_LINUX
#else
    #error Unkown platform.
#endif

// Visual C++
#ifdef _MSC_VER
    #pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#endif

// Build type
#if defined( NDEBUG ) || !defined( _DEBUG )
    #define CURSE_BUILD_RELEASE
#else
    #define CURSE_BUILD_DEBUG
#endif

#if !defined(CURSE_STATIC)
    // Turn off microsoft STL vsc warning
    #ifdef _MSC_VER
        #pragma warning(disable : 4251) // identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
    #endif

    // Define as export or import, if FLARE_EXPORTS is defined.
    #if defined(CURSE_PLATFORM_WINDOWS)
        #if defined(CURSE_EXPORT)
            #define CURSE_API __declspec(dllexport)
        #else
            #define CURSE_API __declspec(dllimport)
        #endif
    #else
        #define CURSE_API
    #endif
#else
    #define CURSE_API
#endif