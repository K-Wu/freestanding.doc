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

#ifndef _SIMT_NEW
#define _SIMT_NEW

#include <new.hxx>

namespace simt { namespace std { inline namespace v1 {

    using bad_alloc = ::std::bad_alloc;
    using bad_array_new_length = ::std::bad_array_new_length;
    using nothrow_t = ::std::nothrow_t;
    static constexpr decltype(::std::nothrow)& nothrow = ::std::nothrow;

    using new_handler = ::std::new_handler;
    inline new_handler set_new_handler(new_handler new_p) noexcept {
        return ::std::set_new_handler(new_p); }
    inline new_handler get_new_handler() noexcept {
        return ::std::get_new_handler(); }

} } }

#endif //_SIMT_NEW
