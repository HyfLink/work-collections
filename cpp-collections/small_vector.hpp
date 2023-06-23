///
/// @file   small_vector.hpp
/// @brief  small_vector.hpp
///
/// @author hyf
/// @date   2022/1/20
///
#pragma once
#include <iterator>
#include <algorithm>
#include <tuple>

#include <vtool/config.hpp>

namespace vt {

    namespace detail {

        template <typename T, size_t N, typename Alloc, size_t Align>
        class _small_vector_base {
        protected:
            using _alloc_traits = std::allocator_traits<Alloc>;

            using allocator_type         = Alloc;
            using size_type              = typename _alloc_traits::size_type;
            using value_type             = typename _alloc_traits::value_type;
            using pointer                = typename _alloc_traits::pointer;
            using reference              = typename _alloc_traits::reference;
            using const_pointer          = typename _alloc_traits::const_pointer;
            using const_reference        = typename _alloc_traits::const_reference;
            using iterator               = pointer;
            using const_iterator         = const_pointer;
            using reverse_iterator       = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            //
            // small_vector has 6 compressed components.
            //     1. (_common_type<0> & ms_flag_mask)
            //          flag bits specifies if the vector is using heap memory.
            //     2. (_common_type<0> & ms_size_mask)
            //          an integer indicates the number of vector elements.
            //     3. _common_type<1>
            //          the allocator, which is compressed with size-flag.
            //     4. _heap_storage<0>
            //          pointer to the beginning of the allocated memory.
            //     5. _heap_storage<1>
            //          pointer to the end of the allocated memory.
            //     6. _stack_storage
            //          uninitialized memory on stack.
            //
            using _common_type   = std::tuple<size_type, allocator_type>;
            using _heap_storage  = std::tuple<pointer, pointer>;
            using _stack_storage = std::aligned_storage_t<sizeof(T) * N, Align>;

            static_assert(N > 0, "Requires a non-zero size hint. Or use std::vector instead.");
            static_assert(Align >= alignof(T), "Requires the Align not less than the default value.");

            static constexpr size_type ms_flag_bit  = sizeof(size_type) * 8 - 1;
            static constexpr size_type ms_flag_mask = static_cast<size_type>(1) << ms_flag_bit;
            static constexpr size_type ms_size_mask = ~ms_flag_mask;

            _common_type m_flag_size_alloc;
            union /* anonymous */ {
                char _m_used_for_construct;
                _heap_storage m_heap_start_finish;
                _stack_storage m_stack_storage;
            };

            //
            // constructor & destructor
            //
            vx_forceinline constexpr _small_vector_base() noexcept
                : m_flag_size_alloc{ 0, allocator_type{} }
                , _m_used_for_construct{} {}

            vx_forceinline constexpr explicit _small_vector_base(allocator_type &&a) noexcept
                : m_flag_size_alloc{ 0, std::move(a) }
                , _m_used_for_construct{} {}

            vx_forceinline constexpr explicit _small_vector_base(allocator_type const &a) noexcept
                : m_flag_size_alloc{ 0, a }
                , _m_used_for_construct{} {}

            //
            // getters
            //

            vx_nodiscard vx_forceinline constexpr bool _on_heap() const noexcept {
                return static_cast<bool>(std::get<0>(m_flag_size_alloc) & ms_flag_mask);
            }

            vx_nodiscard vx_forceinline constexpr allocator_type &_alloc() noexcept {
                return std::get<1>(m_flag_size_alloc);
            }

            vx_nodiscard vx_forceinline constexpr allocator_type const &_alloc() const noexcept {
                return std::get<1>(m_flag_size_alloc);
            }

            vx_nodiscard vx_forceinline constexpr pointer _stack_start() noexcept {
                vx_assert(!_on_heap(), "small vector: stack function called when using heap memory.");
                return reinterpret_cast<pointer>(std::addressof(m_stack_storage));
            }

            vx_nodiscard vx_forceinline constexpr const_pointer _stack_start() const noexcept {
                vx_assert(!_on_heap(), "small vector: stack function called when using heap memory.");
                return reinterpret_cast<const_pointer>(std::addressof(m_stack_storage));
            }

            vx_nodiscard vx_forceinline constexpr size_type _stack_capacity() const noexcept {
                vx_assert(!_on_heap(), "small vector: stack function called when using heap memory.");
                return static_cast<size_type>(N);
            }

            vx_nodiscard vx_forceinline pointer &_heap_start() noexcept {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                return std::get<0>(m_heap_start_finish);
            }

            vx_nodiscard vx_forceinline const_pointer _heap_start() const noexcept {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                return std::get<0>(m_heap_start_finish);
            }

            vx_nodiscard vx_forceinline pointer &_heap_finish() noexcept {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                return std::get<1>(m_heap_start_finish);
            }

            vx_nodiscard vx_forceinline const_pointer _heap_finish() const noexcept {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                return std::get<1>(m_heap_start_finish);
            }

            vx_nodiscard vx_forceinline size_type _heap_capacity() const noexcept {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                return static_cast<size_type>(_heap_finish() - _heap_start());
            }

            vx_nodiscard vx_forceinline constexpr size_type _size() const noexcept {
                return std::get<0>(m_flag_size_alloc) & ms_size_mask;
            }

            vx_nodiscard vx_forceinline constexpr bool _max_size() const noexcept {
                return ms_size_mask;
            }

            //
            // setters
            //

            vx_forceinline void _set_on_heap() noexcept { std::get<0>(m_flag_size_alloc) |= ms_flag_mask; }
            vx_forceinline void _set_on_stack() noexcept { std::get<0>(m_flag_size_alloc) &= ms_size_mask; }
            vx_forceinline void _set_heap_size(size_type n) noexcept { std::get<0>(m_flag_size_alloc) = n | ms_flag_mask; }
            vx_forceinline void _set_stack_size(size_type n) noexcept { std::get<0>(m_flag_size_alloc) = n & ms_size_mask; }

            //
            // allocator & memory
            //

            vx_nodiscard vx_forceinline pointer _allocate(size_type n) { return _alloc_traits::allocate(_alloc(), n); }
            vx_forceinline void _deallocate(pointer p, size_type n) { _alloc_traits::deallocate(_alloc(), p, n); }

            vx_forceinline void _copy_assign_alloc(allocator_type const &alloc) {
                // forward to _copy_assign_alloc(allocator_type, std::true_type)
                //         or _copy_assign_alloc(allocator_type, std::false_type)
                _copy_assign_alloc(alloc, std::bool_constant<_alloc_traits::propagate_on_vector_copy_assignment::value>());
            }

            vx_forceinline void _move_assign_alloc(allocator_type &&alloc) {
                // forward to _move_assign_alloc(allocator_type, std::true_type)
                //         or _move_assign_alloc(allocator_type, std::false_type)
                _move_assign_alloc(std::move(alloc), std::bool_constant<_alloc_traits::propagate_on_vector_move_assignment::value>());
            }

            vx_forceinline void _self_allocate(size_type n) {
                _heap_start()  = _allocate(n);
                _heap_finish() = _heap_start() + n;
            }

            vx_forceinline void _heap_destruct_to_end(pointer new_end) {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                std::destroy(new_end, _heap_start() + _size());
                _set_heap_size(new_end - _heap_start());
            }

            vx_forceinline void _stack_destruct_to_end(pointer new_end) {
                vx_assert(!_on_heap(), "small vector: stack function called when using heap memory.");
                std::destroy(new_end, _stack_start() + _size());
                _set_stack_size(new_end - _stack_start());
            }

            vx_forceinline void _heap_destroy() {
                vx_assert(_on_heap(), "small vector: heap function called when using stack memory.");
                std::destroy_n(_heap_start(), _size());
                _deallocate(_heap_start(), _heap_capacity());
            }

            vx_forceinline void _stack_destroy() {
                vx_assert(!_on_heap(), "small vector: stack function called when using heap memory.");
                std::destroy_n(_stack_start(), _size());
            }

            /// Calculate the recommended capacity.
            vx_nodiscard constexpr size_type _recommend(size_type count) const noexcept {
                const size_type max = this->_max_size();
                const size_type cap = this->_capacity();
                vx_assert(count < this->_max_size(), "small vector: insufficient memory.");
                if (cap > max / 2) return max;
                return std::max(count, 2 * cap);
            }

        private:
            vx_forceinline void _copy_assign_alloc(allocator_type const &alloc, std::true_type) {
                if (!_alloc_traits::is_always_equal::value && _alloc() != alloc && _on_heap()) {
                    std::destroy(_heap_start(), _heap_finish());
                    _deallocate(_heap_start(), _heap_capacity());
                    _heap_start() = _heap_finish() = nullptr;
                    _set_stack_size(0);
                }
                _alloc() = alloc;
            }
            vx_forceinline void _copy_assign_alloc(allocator_type const &, std::false_type) {}
            vx_forceinline void _move_assign_alloc(allocator_type &&alloc, std::true_type) {
                _alloc() = std::move(alloc);
            }
            vx_forceinline void _move_assign_alloc(allocator_type &&, std::false_type) {}
        };

    } // namespace detail

    template <typename T, size_t N, typename Alloc, size_t Align>
    class small_vector : private detail::_small_vector_base<T, N, Alloc, Align> {
        using _small_vector_base = detail::_small_vector_base<T, N, Alloc, Align>;

    public:
        using allocator_type         = typename _small_vector_base::allocator_type;
        using size_type              = typename _small_vector_base::size_type;
        using value_type             = typename _small_vector_base::value_type;
        using pointer                = typename _small_vector_base::pointer;
        using reference              = typename _small_vector_base::reference;
        using const_pointer          = typename _small_vector_base::const_pointer;
        using const_reference        = typename _small_vector_base::const_reference;
        using iterator               = typename _small_vector_base::iterator;
        using const_iterator         = typename _small_vector_base::const_iterator;
        using reverse_iterator       = typename _small_vector_base::reverse_iterator;
        using const_reverse_iterator = typename _small_vector_base::const_reverse_iterator;

        /// @brief Default constructor.
        ///     Construct an empty vector with a default-constructed allocator.
        constexpr small_vector() noexcept
            : _small_vector_base() {}

        /// @brief Construct an empty vector with a given allocator.
        /// @param alloc allocator to use for all memory allocations of this vector.
        constexpr explicit small_vector(allocator_type const &alloc)
            : _small_vector_base(alloc) {}

        /// @brief Construct a vector with count default-constructed elements.
        /// @param count the size of the vector.
        explicit small_vector(size_type count)
            : _small_vector_base() {
            std::uninitialized_default_construct_n(_reserve_for_construct(count), count);
        }

        /// @brief Construct a vector with count default-constructed elements
        ///     and a given allocator.
        /// @param count the size of the vector.
        /// @param alloc allocator to use for all memory allocations of this vector.
        explicit small_vector(size_type count, allocator_type const &alloc)
            : _small_vector_base(alloc) {
            std::uninitialized_default_construct_n(_reserve_for_construct(count), count);
        }

        /// @brief Construct a vector with count copied elements of value.
        /// @param count the size of the vector.
        /// @param value the value to initialize elements of the vector with.
        small_vector(size_type count, const_reference value)
            : _small_vector_base() {
            std::uninitialized_fill_n(_reserve_for_construct(count), count, value);
        }

        /// @brief Construct a vector with count copied elements of value and a
        ///     given allocator.
        /// @param count the size of the vector.
        /// @param value the value to initialize elements of the vector with.
        /// @param alloc allocator to use for all memory allocations of this vector.
        small_vector(size_type count, const_reference value, allocator_type const &alloc)
            : _small_vector_base(alloc) {
            std::uninitialized_fill_n(_reserve_for_construct(count), count, value);
        }

        /// @brief Construct a vector with contents of the initializer list.
        /// @param init initializer list to initialize the vector.
        small_vector(std::initializer_list<value_type> init)
            : _small_vector_base() {
            std::uninitialized_copy_n(init.begin(), init.size(), _reserve_for_construct(init.size()));
        }

        /// @brief Construct a vector with contents of the initializer list and
        ///     a given allocator.
        /// @param init initializer list to initialize the vector.
        /// @param alloc allocator to use for all memory allocations of this vector.
        small_vector(std::initializer_list<value_type> init, allocator_type const &alloc)
            : _small_vector_base(alloc) {
            std::uninitialized_copy_n(init.begin(), init.size(), _reserve_for_construct(init.size()));
        }

        /// @brief Copy constructor.
        ///     Construct a vector with the copy of the contents of other.
        /// @param other another vector to be used as source.
        small_vector(small_vector const &other)
            : _small_vector_base(other._alloc()) {
            std::uninitialized_copy_n(other.data(), other._size(), _reserve_for_construct(other._size()));
        }

        /// @brief Construct a vector with the copy of the contents of other and
        ///     a given allocator.
        /// @param other another vector to be used as source.
        /// @param alloc allocator to use for all memory allocations of this vector.
        small_vector(small_vector const &other, allocator_type const &alloc)
            : _small_vector_base(alloc) {
            std::uninitialized_copy_n(other.data(), other._size(), _reserve_for_construct(other._size()));
        }

        /// @brief Move constructor.
        ///     Construct a vector with the copy of the contents of other.
        /// @note After the move, other is guaranteed to be empty.
        /// @param other another vector to be used as source.
        // small_vector(small_vector &&other) noexcept
        //    : _small_vector_base(std::move(other._alloc())) {}

        /// @brief Construct a vector with the copy of the contents of other and
        ///     a given allocator.
        /// @note After the move, other is guaranteed to be empty.
        /// @param other another vector to be used as source.
        /// @param alloc allocator to use for all memory allocations of this vector.
        // small_vector(small_vector &&other, allocator_type const &alloc)
        //    : _small_vector_base(alloc) {}

        /// @brief Construct a vector with contents of the range [first, last).
        /// @note If InIter is an integral type, the constructor will forward to
        ///     small_vector(size_type count, const_reference value).
        /// @param first iterator to the first element of the range.
        /// @param last  iterator after the last element of the range.
        template <typename InIter>
        vx_forceinline small_vector(InIter first, InIter last)
            : small_vector(first, last, allocator_type(), std::is_integral<InIter>()) {}

        /// @brief Construct a vector with contents of the range [first, last)
        ///     and a given allocator.
        /// @note If InIter is an integral type, the constructor will forward to
        ///     small_vector(size_type count, const_reference value, allocator_type const &alloc).
        /// @param first iterator to the first element of the range.
        /// @param last  iterator after the last element of the range.
        /// @param alloc allocator to use for all memory allocations of this vector.
        template <typename InIter>
        vx_forceinline small_vector(InIter first, InIter last, allocator_type const &alloc)
            : small_vector(first, last, alloc, std::is_integral<InIter>()) {}

        /// @brief Destruct the vector.
        ///     Destruct all elements in the vector, deallocate the memory.
        //        vx_forceinline ~small_vector() {}

        vx_nodiscard vx_forceinline iterator begin() noexcept { return iterator(data()); }
        vx_nodiscard vx_forceinline iterator end() noexcept { return iterator(data() + this->_size()); }
        vx_nodiscard vx_forceinline const_iterator begin() const noexcept { return const_iterator(data()); }
        vx_nodiscard vx_forceinline const_iterator end() const noexcept { return const_iterator(data() + this->_size()); }
        vx_nodiscard vx_forceinline const_iterator cbegin() const noexcept { return const_iterator(data()); }
        vx_nodiscard vx_forceinline const_iterator cend() const noexcept { return const_iterator(data() + this->_size()); }
        vx_nodiscard vx_forceinline reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
        vx_nodiscard vx_forceinline reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
        vx_nodiscard vx_forceinline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
        vx_nodiscard vx_forceinline const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
        vx_nodiscard vx_forceinline const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
        vx_nodiscard vx_forceinline const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }
        vx_nodiscard vx_forceinline bool empty() const noexcept { return (0 == this->_size()); }
        vx_nodiscard vx_forceinline size_type size() const noexcept { return this->_size(); }
        vx_nodiscard vx_forceinline size_type max_size() const noexcept { return this->_max_size(); }

        vx_nodiscard vx_forceinline size_type capacity() const noexcept {
            return this->_on_heap() ? this->_heap_capacity() : this->_stack_capacity();
        }

        vx_nodiscard vx_forceinline allocator_type const &get_allocator() const noexcept {
            return this->_alloc();
        }

        vx_nodiscard vx_forceinline reference front() noexcept {
            vx_assert(0 < this->_size(), "small vector: access value of empty vector.");
            return *data();
        }

        vx_nodiscard vx_forceinline const_reference front() const noexcept {
            vx_assert(0 < this->_size(), "small vector: access value of empty vector.");
            return *data();
        }

        vx_nodiscard vx_forceinline reference back() noexcept {
            vx_assert(0 < this->_size(), "small vector: access value of empty vector.");
            return *(data() + this->_size() - 1);
        }

        vx_nodiscard vx_forceinline const_reference back() const noexcept {
            vx_assert(0 < this->_size(), "small vector: access value of empty vector.");
            return *(data() + this->_size() - 1);
        }

        vx_nodiscard vx_forceinline pointer data() noexcept {
            return this->_on_heap() ? this->_heap_start() : this->_stack_start();
        }

        vx_nodiscard vx_forceinline const_pointer data() const noexcept {
            return this->_on_heap() ? this->_heap_start() : this->_stack_start();
        }

        vx_nodiscard vx_forceinline reference at(size_type n) noexcept {
            vx_assert(n < this->_size(), "small vector: index out of range!");
            return *(data() + n);
        }

        vx_nodiscard vx_forceinline const_reference at(size_type n) const noexcept {
            vx_assert(n < this->_size(), "small vector: index out of range!");
            return *(data() + n);
        }

        vx_nodiscard vx_forceinline reference operator[](size_type n) noexcept {
            vx_assert(n < this->_size(), "small vector: index out of range!");
            return *(data() + n);
        }

        vx_nodiscard vx_forceinline const_reference operator[](size_type n) const noexcept {
            vx_assert(n < this->_size(), "small vector: index out of range!");
            return *(data() + n);
        }

        vx_forceinline void clear() {
            if (this->_on_heap()) {
                _heap_clear();
            } else {
                _stack_clear();
            }
        }

        vx_forceinline void reserve(size_type count) {
            if (this->_on_heap()) {
                _heap_reserve(count);
            } else {
                _stack_reserve(count);
            }
        }

        void shrink_to_fit() {
            if (this->_on_heap()) {
                const auto count   = this->_size();
                const auto old_cap = this->_heap_capacity();
                const auto old_buf = this->_heap_start();
                if (count <= static_cast<size_type>(N) /* this->_stack_capacity() */) {
                    this->_set_on_stack();
                    std::uninitialized_move_n(old_buf, count, this->_stack_start());
                    std::destroy_n(old_buf, count);
                    this->_deallocate(old_buf, old_cap);
                } else if (count < this->_heap_capacity()) {
                    this->_self_allocate(count);
                    std::uninitialized_move_n(old_buf, count, this->_heap_start());
                    std::destroy_n(old_buf, count);
                    this->_deallocate(old_buf, old_cap);
                }
            }
        }

        // small_vector &operator=(small_vector const& other);
        // small_vector &operator=(small_vector &&other);

        small_vector &operator=(std::initializer_list<value_type> init) {
            this->assign(init);
            return *this;
        }

        // void assign(size_type count, const_reference value);
        // void assign(std::initializer_list<value_type> init);

        template <typename InIter>
        vx_forceinline void assign(InIter first, InIter last) {
            assign(std::move(first), std::move(last), std::is_integral<InIter>());
        }

        // insert
        // emplace
        // push_back
        // emplace_back
        // erase
        // pop_back
        // resize
        // swap

    private:
        template <typename Arg>
        vx_forceinline small_vector(Arg count, Arg value, allocator_type const &alloc, std::true_type)
            : small_vector(static_cast<size_type>(count), static_cast<value_type>(value), alloc) {}

        template <typename InIter>
        vx_forceinline small_vector(InIter first, InIter last, allocator_type const &alloc, std::false_type)
            : _small_vector_base(alloc) {
            const size_type count = std::distance(first, last);
            std::uninitialized_copy_n(first, count, _reserve_for_construct(count));
        }

        template <typename Arg>
        vx_forceinline void assign(Arg count, Arg value, std::true_type) {
            assign(static_cast<size_type>(count), static_cast<value_type>(value));
        }

         template <typename InIter>
         void assign(InIter first, InIter last, std::false_type) {}

        /// Reserve memory for some constructors.
        /// Returns a pointer to the memory that needs to be initialized.
        vx_nodiscard pointer _reserve_for_construct(size_type count) {
            if (count > N) {
                this->_set_heap_size(count);
                this->_heap_start()  = this->_allocate(count);
                this->_heap_finish() = this->_heap_start() + count;
                return this->_heap_start();
            } else {
                this->_set_stack_size(count);
                return this->_stack_start();
            }
        }

        void _heap_reserve(size_type count) {
            vx_assert(this->_on_heap(), "small vector: heap function called when using stack memory.");
            if (this->_heap_capacity() < count) {
                const auto buf = this->_allocate(count);
                std::uninitialized_move_n(this->_heap_start(), this->_size(), buf);
                this->_heap_destroy();
                this->_heap_start()  = buf;
                this->_heap_finish() = buf + count;
            }
        }

        void _stack_reserve(size_type count) {
            vx_assert(!this->_on_heap(), "small vector: stack function called when using heap memory.");
            if (this->_stack_capacity() < count) {
                const auto buf = this->_allocate(count);
                std::uninitialized_move_n(this->_stack_start(), this->_size(), buf);
                this->_stack_destroy();
                this->_heap_start()  = buf;
                this->_heap_finish() = buf + count;
                this->_set_on_heap();
            }
        }

        vx_forceinline void _heap_clear() {
            vx_assert(this->_on_heap(), "small vector: heap function called when using stack memory.");
            std::destroy_n(this->_heap_start(), this->_size());
            this->_set_heap_size(0);
        }

        vx_forceinline void _stack_clear() {
            vx_assert(!this->_on_heap(), "small vector: stack function called when using heap memory.");
            std::destroy_n(this->_stack_start(), this->_size());
            this->_set_stack_size(0);
        }
    };

} // namespace vt
