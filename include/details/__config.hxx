/*

Copyright (c) 2018, NVIDIA Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#ifndef __simt__
#define __simt__

#ifdef __CUDACC__
    #if defined(__clang__)
        #include <cuda_fp16.h>
        #define __fp16 __half
    #endif
    #if defined(__FLT16_MANT_DIG__)
        #include <cuda_fp16.h>
        #define _Float16 __half
    #endif
#else
    #include <cuda_runtime_api.h>
#endif

#ifdef _MSC_VER
    #undef __cpp_lib_transparent_operators
#endif

// request these outcomes
#define _LIBCPP_NO_AUTO_LINK
#define _LIBCPP_NO_EXCEPTIONS
#define _LIBCPP_NO_RTTI
#define _LIBCPP_HAS_NO_INT128
#define _LIBCPP_FREESTANDING
#define _LIBCPP_HAS_NO_SEMAPHORES
#define _LIBCPP_HAS_NO_THREAD_CONTENTION_TABLE
#define _LIBCPP_HAS_NO_THREAD_CONTENTION_STATE
#define _LIBCPP_HAS_NO_PLATFORM_WAIT
#define _LIBCPP_HAS_CLOCK_API_EXTERNAL
#ifndef _LIBCUPP_HAS_TREE_BARRIER
# define _LIBCPP_HAS_NO_TREE_BARRIER
#endif
#ifdef __CUDACC_RTC__
    #define __ELF__
    #define _LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER
    #define _LIBCPP_HAS_THREAD_API_EXTERNAL
    #define __alignof(x) alignof(x)
#endif
#define _LIBCPP_HAS_EXTERNAL_ATOMIC_IMP

#include "../../libcxx/include/__config.hxx"

#ifdef __CUDACC_RTC__
#  undef _ALIGNAS_TYPE
#  undef _ALIGNAS
#  define _ALIGNAS_TYPE(x) alignas(x)
#  define _ALIGNAS(x) alignas(x)
#endif

// force this outcome
#undef _LIBCPP_ATOMIC_FLAG_TYPE
#define _LIBCPP_ATOMIC_FLAG_TYPE int
#undef _LIBCPP_INLINE_VISIBILITY
#define _LIBCPP_INLINE_VISIBILITY __host__ __device__
#undef _LIBCPP_FUNC_VIS
#define _LIBCPP_FUNC_VIS _LIBCPP_INLINE_VISIBILITY
#undef _LIBCPP_TYPE_VIS
#define _LIBCPP_TYPE_VIS

// redefine namespace std::
#undef _LIBCPP_BEGIN_NAMESPACE_STD
#define _LIBCPP_BEGIN_NAMESPACE_STD \
    namespace simt { namespace std { inline namespace v1 {
#undef _LIBCPP_END_NAMESPACE_STD
#define _LIBCPP_END_NAMESPACE_STD } } }
#undef _VSTD
#define _VSTD simt::std::v1
#define _LIBCPP_BEGIN_NAMESPACE_STD_NOVERSION namespace simt { namespace std {
#define _LIBCPP_END_NAMESPACE_STD_NOVERSION } }

#endif //__simt__
