///
/// @file   config.hpp
/// @brief  config.hpp
///
/// @author hyf
/// @date   2022/1/20
///
#pragma once
#include <memory>

#ifndef vx_nodiscard
#    define vx_nodiscard [[nodiscard]]
#endif

#ifndef vx_forceinline
#    ifdef _MSC_VER
#        define vx_forceinline inline __force_inline
#    else
#        define vx_forceinline inline __attribute__((always_inline))
#    endif
#endif

#ifndef vx_assert
#    include <cassert>
#    define vx_assert(EXP, MSG) assert((EXP) && (MSG))
#endif

namespace vt {

    template <typename T, typename... Args>
    void construct_at(T *ptr, Args &&...args) noexcept(noexcept(::new (static_cast<void *>(nullptr)) T(std::forward<Args>(args)...))) {
        ::new (const_cast<void *>(static_cast<void const volatile *>(ptr))) T(std::forward<Args>(args)...);
    }

    template <typename T, size_t N, size_t Align = alignof(T)>
    class static_vector;

    template <typename T, size_t N, typename Alloc = std::allocator<T>, size_t Align = alignof(T)>
    class small_vector;

    template <typename T, typename Alloc = std::allocator<T>>
    class stable_vector;

} // namespace vt
