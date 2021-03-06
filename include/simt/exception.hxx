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

#ifndef _SIMT_EXCEPTION
#define _SIMT_EXCEPTION

#include <exception.hxx>

namespace simt { namespace std { inline namespace v1 {

    using exception = ::std::exception;
    using bad_exception = ::std::bad_exception;
    using nested_exception = ::std::nested_exception;

    using unexpected_handler = ::std::unexpected_handler;
    inline unexpected_handler get_unexpected() noexcept { 
        return ::std::get_unexpected(); }
    inline unexpected_handler set_unexpected(unexpected_handler f) noexcept  { 
        return ::std::set_unexpected(f); }
    [[noreturn]] inline void unexpected() {
        ::std::unexpected(); }
 
    using terminate_handler = ::std::terminate_handler;
    inline terminate_handler get_terminate() noexcept { 
        return ::std::get_terminate(); }
    inline terminate_handler set_terminate(terminate_handler f) noexcept { 
        return ::std::set_terminate(f); }
    [[noreturn]] inline void terminate() noexcept { 
        ::std::terminate(); }
 
    inline bool uncaught_exception() noexcept { 
        return ::std::uncaught_exception(); }
 
    using exception_ptr = ::std::exception_ptr;
    inline exception_ptr current_exception() noexcept { 
        return ::std::current_exception(); }
    [[noreturn]] inline void rethrow_exception(exception_ptr p) { 
        ::std::rethrow_exception(p); }
    template<class E> inline exception_ptr make_exception_ptr(E e) noexcept { 
        return ::std::make_exception_ptr(e); }
 
    template <class T> [[noreturn]] inline void throw_with_nested(T&& t) { 
        ::std::throw_with_nested(t); }
    template <class E> inline void rethrow_if_nested(const E& e) { 
        ::std::rethrow_if_nested(e); }

} } }

#endif //_SIMT_EXCEPTION
