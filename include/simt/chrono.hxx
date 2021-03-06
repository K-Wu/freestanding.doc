
/*

Copyright (c) 2019, NVIDIA Corporation

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

#ifndef _SIMT_CHRONO
#define _SIMT_CHRONO

#ifndef __CUDACC_RTC__
    #include <chrono.hxx>
#endif //__CUDACC_RTC__

#include "ctime.hxx"
#include "type_traits.hxx"
#include "ratio.hxx"
#include "limits.hxx"
#include "version.hxx"

#include "../details/__config.hxx"

#include "../../libcxx/include/chrono.hxx"

_LIBCPP_BEGIN_NAMESPACE_STD

namespace chrono {

class _LIBCPP_TYPE_VIS system_clock
{
public:
    typedef nanoseconds                      duration;
    typedef duration::rep                    rep;
    typedef duration::period                 period;
    typedef chrono::time_point<system_clock> time_point;
    static _LIBCPP_CONSTEXPR_AFTER_CXX11 const bool is_steady = false;

    inline _LIBCPP_INLINE_VISIBILITY
    static time_point now() _NOEXCEPT 
    {
#ifdef __CUDA_ARCH__
        uint64_t time;
        asm("mov.u64  %0, %globaltimer;":"=l"(time)::);
        return time_point(nanoseconds(time));
#else
        return time_point(nanoseconds(
                ::std::chrono::duration_cast<::std::chrono::nanoseconds>(
                    ::std::chrono::system_clock::now().time_since_epoch()
                ).count()
               ));
#endif
    }
    inline _LIBCPP_INLINE_VISIBILITY
    static time_t to_time_t(const time_point& __t) _NOEXCEPT 
    {
        return time_t(duration_cast<seconds>(__t.time_since_epoch()).count());
    }
    inline _LIBCPP_INLINE_VISIBILITY
    static time_point from_time_t(time_t __t) _NOEXCEPT
    {
        return time_point(seconds(__t));;
    }
};

using steady_clock = system_clock;
using high_resolution_clock = steady_clock;

#if _LIBCPP_STD_VER > 17

template <class _Duration>
using sys_time    = time_point<system_clock, _Duration>;
using sys_seconds = sys_time<seconds>;
using sys_days    = sys_time<days>;

#endif

}

_LIBCPP_END_NAMESPACE_STD

#endif //_SIMT_CHRONO


