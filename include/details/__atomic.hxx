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

#include <assert.h>
#include <string.h>

#ifndef ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_BOOL_LOCK_FREE      2
#define ATOMIC_CHAR_LOCK_FREE      2
#define ATOMIC_CHAR16_T_LOCK_FREE  2
#define ATOMIC_CHAR32_T_LOCK_FREE  2
#define ATOMIC_WCHAR_T_LOCK_FREE   2
#define ATOMIC_SHORT_LOCK_FREE     2
#define ATOMIC_INT_LOCK_FREE       2
#define ATOMIC_LONG_LOCK_FREE      2
#define ATOMIC_LLONG_LOCK_FREE     2
#define ATOMIC_POINTER_LOCK_FREE   2
#endif //ATOMIC_BOOL_LOCK_FREE

#ifndef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED 0
#define __ATOMIC_CONSUME 1
#define __ATOMIC_ACQUIRE 2
#define __ATOMIC_RELEASE 3
#define __ATOMIC_ACQ_REL 4
#define __ATOMIC_SEQ_CST 5
#endif //__ATOMIC_RELAXED

#ifndef __ATOMIC_BLOCK
#define __ATOMIC_SYSTEM 0 // 0 indicates default
#define __ATOMIC_DEVICE 1
#define __ATOMIC_BLOCK 2
#endif //__ATOMIC_BLOCK


namespace simt {
 
namespace details { inline namespace v1 {

    inline __device__ int __stronger_order_simt(int a, int b) {
        int const max = a > b ? a : b;
        if(max != __ATOMIC_RELEASE)
            return max;
        static int const xform[] = { 
            __ATOMIC_RELEASE, 
            __ATOMIC_ACQ_REL, 
            __ATOMIC_ACQ_REL, 
            __ATOMIC_RELEASE };
        return xform[a < b ? a : b];
    }
} }

inline namespace v1 {

    enum thread_scope {
        thread_scope_system = __ATOMIC_SYSTEM,
        thread_scope_device = __ATOMIC_DEVICE, 
        thread_scope_block = __ATOMIC_BLOCK        
    };

    #define _LIBCPP_ATOMIC_SCOPE_TYPE ::simt::thread_scope
    #define _LIBCPP_ATOMIC_SCOPE_DEFAULT ::simt::thread_scope::system
}

namespace details {inline namespace v1 {

    struct __thread_scope_block_tag { };
    struct __thread_scope_device_tag { };
    struct __thread_scope_system_tag { };

    template<int _S>  struct __scope_enum_to_tag { };
    template<> struct __scope_enum_to_tag<(int)thread_scope_block> {
        using type = __thread_scope_block_tag; };
    template<> struct __scope_enum_to_tag<(int)thread_scope_device> {
        using type = __thread_scope_device_tag; };
    template<> struct __scope_enum_to_tag<(int)thread_scope_system> {
        using type = __thread_scope_system_tag; };

    template<int _S>
    __host__ __device__ auto constexpr __scope_tag() -> 
            typename __scope_enum_to_tag<_S>::type {
        return typename __scope_enum_to_tag<_S>::type();
    }
} }

}

#include "__atomic_generated.hxx"
#include "__atomic_derived.hxx"

_LIBCPP_BEGIN_NAMESPACE_STD

__host__ __device__ inline bool __cxx_atomic_is_lock_free(size_t x) { 
    return x <= 8; 
}
__host__ __device__ inline void __cxx_atomic_thread_fence(int __order) {
#ifdef __CUDA_ARCH__
    details::__atomic_thread_fence_simt(__order, details::__thread_scope_system_tag()); 
#else
    ::std::atomic_thread_fence((::std::memory_order)__order);
#endif
}
__host__ __device__ inline void __cxx_atomic_signal_fence(int __order) {
#ifdef __CUDA_ARCH__
    details::__atomic_signal_fence_simt(__order); 
#else
    ::std::atomic_signal_fence((::std::memory_order)__order);
#endif
}

template <typename _Tp, int _Sco>
struct __cxx_atomic_base_impl_default {

  __host__ __device__ __cxx_atomic_base_impl_default() : __a_value() {
  }
  __host__ __device__ constexpr explicit __cxx_atomic_base_impl_default(_Tp value) : __a_value(value) {
  }
#ifdef __CUDA_ARCH__
  _Tp __a_value;
#else
  ::std::atomic<_Tp> __a_value;
#endif
};

template<class _Tp, int _Sco> 
__host__ __device__ inline void __cxx_atomic_init(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __val) { 
#ifdef __CUDA_ARCH__
    __a->__a_value = __val;
#else
    ::std::atomic_init(&__a->__a_value, __val);
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline void __cxx_atomic_store(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __val, int __order) {
#ifdef __CUDA_ARCH__
    details::__atomic_store_n_simt(&__a->__a_value, __val, __order, details::__scope_tag<_Sco>()); 
#else
    ::std::atomic_store_explicit(&__a->__a_value, __val, (::std::memory_order)__order);
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_load(__cxx_atomic_base_impl_default<_Tp, _Sco> const volatile* __a, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_load_n_simt(&__a->__a_value, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_load_explicit(&__a->__a_value, (::std::memory_order)__order);
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_exchange(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __value, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_exchange_n_simt(&__a->__a_value, __value, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_exchange_explicit(&__a->__a_value, __value, (::std::memory_order)__order); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline bool __cxx_atomic_compare_exchange_strong(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp* __expected, _Tp __value, int __success, int __failure) {
#ifdef __CUDA_ARCH__
    return details::__atomic_compare_exchange_n_simt(&__a->__a_value, __expected, __value, false, __success, __failure, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_compare_exchange_strong_explicit(&__a->__a_value, __expected, __value, (::std::memory_order)__success, (::std::memory_order)__failure); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline bool __cxx_atomic_compare_exchange_weak(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp* __expected, _Tp __value, int __success, int __failure) {
#ifdef __CUDA_ARCH__
    return details::__atomic_compare_exchange_n_simt(&__a->__a_value, __expected, __value, true, __success, __failure, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_compare_exchange_weak_explicit(&__a->__a_value, __expected, __value, (::std::memory_order)__success, (::std::memory_order)__failure); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_add(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __delta, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_fetch_add_simt(&__a->__a_value, __delta, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_fetch_add_explicit(&__a->__a_value, __delta, (::std::memory_order)__order); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp* __cxx_atomic_fetch_add(__cxx_atomic_base_impl_default<_Tp*, _Sco> volatile* __a, ptrdiff_t __delta, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_fetch_add_simt(&__a->__a_value, __delta, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_fetch_add_explicit(&__a->__a_value, __delta, (::std::memory_order)__order); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_sub(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __delta, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_fetch_sub_simt(&__a->__a_value, __delta, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_fetch_sub_explicit(&__a->__a_value, __delta, (::std::memory_order)__order); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_and(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __pattern, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_fetch_and_simt(&__a->__a_value, __pattern, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_fetch_and_explicit(&__a->__a_value, __pattern, (::std::memory_order)__order); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_or(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __pattern, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_fetch_or_simt(&__a->__a_value, __pattern, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_fetch_or_explicit(&__a->__a_value, __pattern, (::std::memory_order)__order); 
#endif
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_xor(__cxx_atomic_base_impl_default<_Tp, _Sco> volatile* __a, _Tp __pattern, int __order) {
#ifdef __CUDA_ARCH__
    return details::__atomic_fetch_xor_simt(&__a->__a_value, __pattern, __order, details::__scope_tag<_Sco>()); 
#else
    return ::std::atomic_fetch_xor_explicit(&__a->__a_value, __pattern, (::std::memory_order)__order); 
#endif
}

template <typename _Tp, int _Sco>
struct __cxx_atomic_base_impl_small {

  __host__ __device__ __cxx_atomic_base_impl_small() : __a_value() {
  }
  __host__ __device__ constexpr explicit __cxx_atomic_base_impl_small(_Tp value) : __a_value(value) {
  }

  __cxx_atomic_base_impl_default<uint32_t, _Sco> __a_value;
};

template <typename _Tp>
using __cxx_small_proxy = typename conditional<sizeof(_Tp) == 1,
                                               uint8_t, 
                                               typename conditional<sizeof(_Tp) == 2,
                                                                    uint16_t,
                                                                    void>::type >::type;

template<class _Tp> 
__host__ __device__ inline uint32_t __cxx_small_to_32(_Tp __val) { 
    __cxx_small_proxy<_Tp> __temp;
    memcpy(&__temp, &__val, sizeof(_Tp));
    return __temp;
}

template<class _Tp> 
__host__ __device__ inline _Tp __cxx_small_from_32(uint32_t __val) { 
    __cxx_small_proxy<_Tp> __temp = __val;
    _Tp __result;
    memcpy(&__result, &__temp, sizeof(_Tp));
    return __result;
}

template<class _Tp, int _Sco> 
__host__ __device__ inline void __cxx_atomic_init(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __val) { 
    __cxx_atomic_init(&__a->__a_value, __cxx_small_to_32(__val));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline void __cxx_atomic_store(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __val, int __order) {
    __cxx_atomic_store(&__a->__a_value, __cxx_small_to_32(__val), __order);
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_load(__cxx_atomic_base_impl_small<_Tp, _Sco> const volatile* __a, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_load(&__a->__a_value, __order));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_exchange(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __value, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_exchange(&__a->__a_value, __cxx_small_to_32(__value), __order));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline bool __cxx_atomic_compare_exchange_weak(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp* __expected, _Tp __value, int __success, int __failure) {
    auto __temp = __cxx_small_to_32(*__expected);
    auto const __ret = __cxx_atomic_compare_exchange_weak(&__a->__a_value, &__temp, __cxx_small_to_32(__value), __success, __failure);
    auto const __actual = __cxx_small_from_32<_Tp>(__temp);
    if(!__ret) {
        if(0 == memcmp(&__actual, __expected, sizeof(_Tp)))
            __cxx_atomic_fetch_and(&__a->__a_value, (1u << (8*sizeof(_Tp))) - 1, __ATOMIC_RELAXED);
        else
            *__expected = __actual;
    }
    return __ret;
}
template<class _Tp, int _Sco> 
__host__ __device__ inline bool __cxx_atomic_compare_exchange_strong(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp* __expected, _Tp __value, int __success, int __failure) {
    auto const __old = *__expected;
    while(1) {
        if(__cxx_atomic_compare_exchange_weak(__a, __expected, __value, __success, __failure))
            return true;
        if(0 != memcmp(&__old, __expected, sizeof(_Tp)))
            return false;
    }
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_add(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __delta, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_fetch_add(&__a->__a_value, __cxx_small_to_32(__delta), __order));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_sub(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __delta, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_fetch_sub(&__a->__a_value, __cxx_small_to_32(__delta), __order));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_and(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __pattern, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_fetch_and(&__a->__a_value, __cxx_small_to_32(__pattern), __order));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_or(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __pattern, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_fetch_or(&__a->__a_value, __cxx_small_to_32(__pattern), __order));
}
template<class _Tp, int _Sco> 
__host__ __device__ inline _Tp __cxx_atomic_fetch_xor(__cxx_atomic_base_impl_small<_Tp, _Sco> volatile* __a, _Tp __pattern, int __order) {
    return __cxx_small_from_32<_Tp>(__cxx_atomic_fetch_xor(&__a->__a_value, __cxx_small_to_32(__pattern), __order));
}

template <typename _Tp, int _Sco>
using __cxx_atomic_base_impl = typename conditional<sizeof(_Tp) < 4,
                                    __cxx_atomic_base_impl_small<_Tp, _Sco>,
                                    __cxx_atomic_base_impl_default<_Tp, _Sco> >::type;

_LIBCPP_END_NAMESPACE_STD
