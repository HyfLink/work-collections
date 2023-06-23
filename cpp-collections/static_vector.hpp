///
/// @file   static_vector.hpp
/// @brief  static_vector.hpp
///
/// @author hyf
/// @date   2022/1/20
///
#pragma once
#include <iterator>
#include <algorithm>

#include <vtool/config.hpp>

namespace vt {

    template <typename T, size_t N, size_t Align>
    class static_vector {
    public:
        using size_type              = std::size_t;
        using value_type             = T;
        using pointer                = T *;
        using reference              = T &;
        using const_pointer          = const T *;
        using const_reference        = const T &;
        using iterator               = pointer;
        using const_iterator         = const_pointer;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    private:
        using storage_type = std::aligned_storage_t<sizeof(T) * N, Align>;

        size_type m_size = static_cast<size_type>(0);
        storage_type m_storage;

    public:
        /// @brief Default constructor.
        ///     construct an empty vector.
        constexpr static_vector() noexcept = default;

        /// @brief Move constructor.
        ///     `x` will be empty after move.
        /// @param x Another vector.
        constexpr static_vector(static_vector &&x) noexcept
            : m_size(x.size()) {
            std::uninitialized_move_n(x.data(), x.size(), data());
            x.m_size = 0;
        }

        /// @brief Copy constructor.
        /// @param x Another vector.
        constexpr static_vector(static_vector const &x) noexcept
            : m_size(x.size()) {
            std::uninitialized_copy_n(x.data(), x.size(), data());
        }

        /// @brief Construct with `n` default values.
        /// @note  Requires `n` is not greater than `capacity()`.
        /// @param n Number of elements to construct.
        constexpr explicit static_vector(size_type n) noexcept
            : m_size(n) {
            vx_assert(n <= capacity(), "static_vector: insufficient capacity.");
            std::uninitialized_default_construct_n(data(), n);
        }

        /// @brief Construct with `n` elements of `value`.
        /// @note  Requires `n` is not greater than `capacity()`.
        /// @param n     Number of elements to construct.
        /// @param value Element value to be copied.
        constexpr static_vector(size_type n, const_reference value) noexcept
            : m_size(n) {
            vx_assert(n <= capacity(), "static_vector: insufficient capacity.");
            std::uninitialized_fill_n(data(), n, value);
        }

        /// @brief Construct from an initializer list
        /// @note  Requires the size of `il` is not greater than `capacity()`.
        /// @param il The initializer list.
        constexpr static_vector(std::initializer_list<T> il) noexcept
            : m_size(il.size()) {
            vx_assert(il.size() <= capacity(), "static_vector: insufficient capacity.");
            std::uninitialized_copy_n(il.begin(), il.size(), data());
        }

        /// @brief Construct from values in range `first` to `last`.
        /// @note  Requires the size of the range is not greater than `capacity()`.
        /// @tparam InIter Input iterator.
        /// @param first   Iterator that indicates the start of the range.
        /// @param last    Iterator that indicates the end of the range.
        template <typename InIter>
        constexpr static_vector(InIter first, InIter last) noexcept
            : static_vector(first, last, std::is_integral<InIter>()) {
            // If `InIter` is an integral type,
            //      Forward to `static_vector(Arg, Arg, std::true_type)`.
            //      Forward to `static_vector(size_type, const_reference)`.
            // If `InIter` is an iterator.
            //      Forward to `static_vector(InIter, InIter, std::false_type)`,
        }

        /// @brief Destructor.
        ~static_vector() noexcept {
            std::destroy_n(data(), size());
        }

        /// @brief Get the pointer to the vector data.
        /// @return pointer
        vx_nodiscard vx_forceinline constexpr pointer data() noexcept {
            return reinterpret_cast<pointer>(&m_storage);
        }

        /// @brief Get the pointer to the vector data.
        /// @return const_pointer
        vx_nodiscard vx_forceinline constexpr const_pointer data() const noexcept {
            return reinterpret_cast<const_pointer>(&m_storage);
        }

        /// @brief Check if the vector is empty.
        ///     Same as vector.size() == 0.
        /// @return bool
        vx_nodiscard vx_forceinline constexpr bool empty() const noexcept { return m_size == 0; }

        /// @brief Get the number of vector elements.
        /// @return size_type
        vx_nodiscard vx_forceinline constexpr size_type size() const noexcept { return m_size; }

        /// @brief Get the maximum number of vector elements.
        ///     For a static vector, max_size() always equals to the template parameter N.
        /// @return size_type
        vx_nodiscard vx_forceinline constexpr size_type max_size() const noexcept { return N; }

        /// @brief Get the capacity of the vector.
        ///     For a static vector, capacity() always equals to the template parameter N.
        /// @return size_type
        vx_nodiscard vx_forceinline constexpr size_type capacity() const noexcept { return N; }

        /// @brief  Get the iterator indicates the start of the vector.
        /// @return iterator
        vx_nodiscard constexpr iterator begin() noexcept { return { data() }; }

        /// @brief Get the iterator indicates the end of the vector.
        /// @return iterator
        vx_nodiscard constexpr iterator end() noexcept { return { data() + size() }; }

        /// @brief  Get the iterator indicates the start of the vector.
        /// @return const_iterator
        vx_nodiscard constexpr const_iterator begin() const noexcept { return { data() }; }

        /// @brief  Get the iterator indicates the end of the vector.
        /// @return const_iterator
        vx_nodiscard constexpr const_iterator end() const noexcept { return { data() + size() }; }

        /// @brief  Get the const iterator indicates the start of the vector.
        /// @return const_iterator
        vx_nodiscard constexpr const_iterator cbegin() const noexcept { return { data() }; }

        /// @brief  Get the const iterator indicates the end of the vector.
        /// @return const_iterator
        vx_nodiscard constexpr const_iterator cend() const noexcept { return { data() + size() }; }

        /// @brief  Get the reverse iterator indicates the start of the vector.
        /// @return reverse_iterator
        vx_nodiscard constexpr reverse_iterator rbegin() noexcept { return { end() }; }

        /// @brief  Get the reverse iterator indicates the end of the vector.
        /// @return reverse_iterator
        vx_nodiscard constexpr reverse_iterator rend() noexcept { return { begin() }; }

        /// @brief  Get the reverse iterator indicates the start of the vector.
        /// @return const_reverse_iterator
        vx_nodiscard constexpr const_reverse_iterator rbegin() const noexcept { return { cend() }; }

        /// @brief  Get the reverse iterator indicates the end of the vector.
        /// @return const_reverse_iterator
        vx_nodiscard constexpr const_reverse_iterator rend() const noexcept { return { cbegin() }; }

        /// @brief  Get the const reverse iterator indicates the start of the vector.
        /// @return const_reverse_iterator
        vx_nodiscard constexpr const_reverse_iterator crbegin() const noexcept { return { cend() }; }

        /// @brief  Get the const reverse iterator indicates the end of the vector.
        /// @return const_reverse_iterator
        vx_nodiscard constexpr const_reverse_iterator crend() const noexcept { return { cbegin() }; }

        /// @brief  Access elements by index.
        /// @note   Requires index `n` is less than `size()`.
        /// @return reference
        vx_nodiscard constexpr reference operator[](size_type n) noexcept {
            vx_assert(n < size(), "static_vector: index out of range.");
            return *(data() + n);
        }

        /// @brief  Access elements by index.
        /// @note   Requires index `n` is less than `size()`.
        /// @return const_reference
        vx_nodiscard constexpr const_reference operator[](size_type n) const noexcept {
            vx_assert(n < size(), "static_vector: index out of range.");
            return *(data() + n);
        }

        /// @brief  Access elements by index.
        /// @note   Requires index `n` is less than `size()`.
        /// @note   This function won't throw an exception.
        /// @return reference
        vx_nodiscard constexpr reference at(size_type n) noexcept {
            vx_assert(n < size(), "static_vector: index out of range.");
            return *(data() + n);
        }

        /// @brief  Access elements by index.
        /// @note   Requires index `n` is less than `size()`.
        /// @note   This function won't throw an exception.
        /// @return const_reference
        vx_nodiscard constexpr const_reference at(size_type n) const noexcept {
            vx_assert(n < size(), "static_vector: index out of range.");
            return *(data() + n);
        }

        /// @brief  Access the first element in the vector.
        /// @note   Requires the vector is not empty.
        /// @return reference
        vx_nodiscard constexpr reference front() noexcept {
            vx_assert(!empty(), "static_vector: access value in empty vector.");
            return *(data());
        }

        /// @brief  Access the first element in the vector.
        /// @note   Requires the vector is not empty.
        /// @return const_reference
        vx_nodiscard constexpr const_reference front() const noexcept {
            vx_assert(!empty(), "static_vector: access value in empty vector.");
            return *(data());
        }

        /// @brief  Access the last element in the vector.
        /// @note   Requires the vector is not empty.
        /// @return reference
        vx_nodiscard constexpr reference back() noexcept {
            vx_assert(!empty(), "static_vector: access value in empty vector.");
            return *(data() + size() - 1);
        }

        /// @brief  Access the last element in the vector.
        /// @note   Requires the vector is not empty.
        /// @return const_reference
        vx_nodiscard constexpr const_reference back() const noexcept {
            vx_assert(!empty(), "static_vector: access value in empty vector.");
            return *(data() + size() - 1);
        }

        /// @brief Move assignment.
        /// @param other Another vector
        /// @return static_vector &, reference to `*this`.
        constexpr static_vector &operator=(static_vector &&other) noexcept {
            const size_type size1 = this->size();
            const size_type size2 = other.size();

            if (this != &other) {
                if (size2 < size1) {
                    // move other's elements to this.
                    // destroy extra elements in this.
                    const auto it = std::move(other.data(), other.data() + size2, this->data());
                    std::destroy_n(it, size1 - size2);
                } else if (size1 == size2) {
                    std::move(other.data(), other.data() + size2, this->data());
                } else /* size1 < size2 */ {
                    const auto it = std::move(other.data(), other.data() + size1, this->data());
                    std::uninitialized_move_n(other.data() + size1, size2 - size1, it);
                }

                this->m_size = size2;
                other.m_size = 0;
            }
            return *this;
        }

        /// @brief Copy assignment.
        /// @param other Another vector
        /// @return static_vector &, reference to `*this`.
        constexpr static_vector &operator=(static_vector const &other) noexcept {
            // Forward to assign(InIter, InIter, std::false_type)
            if (this != &other) { assign(other.begin(), other.end(), std::false_type()); }
            return *this;
        }

        /// @brief Assign from initializer list.
        /// @param il The initializer list.
        /// @return static_vector &, reference to `*this`.
        constexpr static_vector &operator=(std::initializer_list<T> il) noexcept {
            // Forward to assign(InIter, InIter, std::false_type)
            vx_assert(il.size() <= capacity(), "static_vector: insufficient capacity.");
            assign(il.begin(), il.end(), std::false_type());
            return *this;
        }

        /// @brief  Assign values in range first to last.
        /// @note   Requires the size of the range is not greater than `capacity()`.
        /// @tparam InIter Input Iterator.
        /// @param  first  Iterator that indicates the start of the range.
        /// @param  last   Iterator that indicates the end of the range.
        template <typename InIter>
        constexpr void assign(InIter first, InIter last) noexcept {
            // If `InIter` is an integral type,
            //      forward to assign(first, last, std::true_type).
            // If `InIter` is an input iterator,
            //      forward to assign(first, last, std::false_type).
            assign(first, last, std::is_integral<InIter>());
        }

        /// @brief Assign `n` elements of `value`.
        /// @note  Requires `n` is not greater than `capacity()`.
        /// @param n     Number of elements to construct.
        /// @param value Element value to be copied.
        constexpr void assign(size_type n, const_reference value) noexcept {
            vx_assert(n <= capacity(), "static_vector: insufficient capacity.");

            if (n < size()) {
                const auto it = std::fill_n(data(), n, value);
                std::destroy_n(it, size() - n);
                m_size = n;
            } else if (n == size()) {
                std::fill_n(data(), n, value);
            } else /* n > size() */ {
                const auto it = std::fill_n(data(), size(), value);
                std::uninitialized_fill_n(it, n - size(), value);
                m_size = n;
            }
        }

        /// @brief Assign from initializer list.
        /// @note  Requires the size of the list is not greater than `capacity()`.
        /// @param il The initializer list.
        constexpr void assign(std::initializer_list<T> il) noexcept {
            vx_assert(il.size() <= capacity(), "static_vector: insufficient capacity.");
            assign(il.begin(), il.end());
        }

        /// @brief Clear all elements in the vector.
        constexpr void clear() noexcept {
            if (!empty()) {
                std::destroy_n(data(), size());
                m_size = 0;
            }
        }

        /// @brief Resize the vector.
        ///     If n is equal to the size of the vector, do nothing.
        ///     If n is greater than the size, append default values.
        ///     If n is less than the size, erase extra elements.
        /// @note Requires `n` is not greater than `capacity()`.
        constexpr void resize(size_type n) noexcept {
            vx_assert(n <= capacity(), "static_vector: insufficient capacity.");

            if (n < m_size) {
                std::destroy_n(data() + n, size() - n);
            } else if (n > m_size) {
                std::uninitialized_default_construct_n(data() + size(), n - m_size);
            }

            m_size = n;
        }

        /// @brief Resize the vector.
        ///     If n is equal to the size of the vector, do nothing.
        ///     If n is greater than the size, append values `c`.
        ///     If n is less than the size, erase extra elements.
        /// @note Requires `n` is not greater than `capacity()`.
        constexpr void resize(size_type n, const_reference c) noexcept {
            vx_assert(n <= capacity(), "static_vector: insufficient capacity.");

            if (n < m_size) {
                std::destroy_n(data() + n, m_size - n);
            } else if (n > m_size) {
                std::uninitialized_fill_n(data() + size(), n - m_size, c);
            }

            m_size = n;
        }

        /// @brief Swap with another vector.
        /// @param other Another vector.
        constexpr void swap(static_vector &other) {
            if (this != &other) {
                pointer first1 = this->begin(), last1 = this->end();
                pointer first2 = other.begin(), last2 = other.end();

                for (; first1 < last1 && first2 < last2; ++first1, ++first2) {
                    std::iter_swap(first1, first2);
                }

                if (first1 < last1) {
                    std::uninitialized_move(first1, last1, first2);
                    std::swap(m_size, other.m_size);
                } else if (first2 < last2) {
                    std::uninitialized_move(first2, last2, first1);
                    std::swap(m_size, other.m_size);
                }
            }
        }

        /// @brief push `x` to the end of the vector.
        /// @note  Requires the vector is not full.
        /// @param x An element.
        constexpr void push_back(const_reference x) {
            vx_assert(size() < capacity(), "static_vector: insufficient capacity.");
            construct_at(data() + size(), x);
            ++m_size;
        }

        /// @brief push `x` to the end of the vector.
        /// @note  Requires the vector is not full.
        /// @param x An element.
        constexpr void push_back(value_type &&x) {
            vx_assert(size() < capacity(), "static_vector: insufficient capacity.");
            construct_at(data() + size(), std::move(x));
            ++m_size;
        }

        /// @brief pop the last element of the vector.
        /// @note  Requires the vector is not empty.
        constexpr void pop_back() {
            vx_assert(!empty(), "static_vector: pop an empty vector.");
            std::destroy_at(data() + (size() - 1));
            --m_size;
        }

        /// @brief  Construct an element at the end of the vector.
        /// @note   Requires the vector is not full.
        /// @tparam Args Type of args.
        /// @param  args Arguments to construct the element.
        /// @return reference, Reference to the element.
        template <typename... Args>
        constexpr reference emplace_back(Args &&...args) {
            vx_assert(size() < capacity(), "static_vector: insufficient capacity.");
            construct_at(data() + size(), std::forward<Args>(args)...);
            ++m_size;
            return back();
        }

        /// @brief  Construct an element at `position`.
        /// @note   Requires the vector is not full.
        /// @note   Requires `position` is an iterator within the vector.
        /// @tparam Args Type of args.
        /// @param  args Arguments to construct the element.
        /// @return iterator, Iterator to the element.
        template <typename... Args>
        constexpr iterator emplace(const_iterator position, Args &&...args) {
            vx_assert(size() < capacity(), "static_vector: insufficient capacity.");
            vx_assert(begin() <= position, "static_vector: invalid iterator.");
            vx_assert(position <= end(), "static_vector: invalid iterator.");

            pointer first  = data() + (position - begin());
            pointer last   = data() + size();
            pointer result = last + 1;

            while (first != last) {
                construct_at(--result, std::move(*--last));
            }

            construct_at(first, std::forward<Args>(args)...);

            ++m_size;
            return iterator(first);
        }

        /// @brief  Insert an element at `position`.
        /// @note   Requires the vector is not full.
        /// @note   Requires `position` is an iterator within the vector.
        /// @param  x An element.
        /// @return iterator, Iterator to the element.
        constexpr iterator insert(const_iterator position, const_reference x) {
            vx_assert(size() < capacity(), "static_vector: insufficient capacity.");
            vx_assert(begin() <= position, "static_vector: invalid iterator.");
            vx_assert(position <= end(), "static_vector: invalid iterator.");

            pointer first  = data() + (position - begin());
            pointer last   = data() + size();
            pointer result = last + 1;

            while (first != last) {
                construct_at(--result, std::move(*--last));
            }

            construct_at(first, x);

            ++m_size;
            return iterator(first);
        }

        /// @brief  Insert an element at `position`.
        /// @note   Requires the vector is not full.
        /// @note   Requires `position` is an iterator within the vector.
        /// @param  x An element.
        /// @return iterator, Iterator to the element.
        constexpr iterator insert(const_iterator position, value_type &&x) {
            vx_assert(size() < capacity(), "static_vector: insufficient capacity.");
            vx_assert(begin() <= position, "static_vector: invalid iterator.");
            vx_assert(position <= end(), "static_vector: invalid iterator.");

            pointer first  = data() + (position - begin());
            pointer last   = data() + size();
            pointer result = last + 1;

            while (first != last) {
                construct_at(--result, std::move(*--last));
            }

            construct_at(first, std::move(x));

            ++m_size;
            return iterator(first);
        }

        /// @brief  Insert elements in range `first` to `last` at `position`.
        /// @note   Requires the vector has enough capacity.
        /// @note   Requires `position` is an iterator within the vector.
        /// @tparam InIter Input Iterator.
        /// @param  first  Iterator that indicates the start of the range.
        /// @param  last   Iterator that indicates the end of the range.
        /// @return iterator, Iterator to the first element.
        template <typename InIter>
        constexpr iterator insert(const_iterator position, InIter first, InIter last) {
            // If `InIter` is an integral type,
            //      forward to `insert(position, InIter, InIter, std::true_type)`.
            //      forward to `insert(position, size_type, const_reference)`.
            // If `InIter` is input iterator,
            //      forward to `insert(position, InIter, InIter, std::false_type)`.
            return insert(position, first, last, std::is_integral<InIter>());
        }

        /// @brief  Insert elements in the initializer list at `position`.
        /// @note   Requires the vector has enough capacity.
        /// @note   Requires `position` is an iterator within the vector.
        /// @param  il  The initializer list.
        /// @return iterator, Iterator to the first element.
        constexpr iterator insert(const_iterator position, std::initializer_list<T> il) {
            // Forward to `insert(position, InIter, InIter, std::false_type)`.
            return insert(position, il.begin(), il.end(), std::false_type());
        }

        /// @brief  Insert `n` elements of `x` at `position`.
        /// @note   Requires the vector has enough capacity.
        /// @note   Requires `position` is an iterator within the vector.
        /// @param  n The number of the elements.
        /// @param  x The value of the elements.
        /// @return iterator, Iterator to the first element.
        constexpr iterator insert(const_iterator position, size_type n, const_reference x) {
            vx_assert(n + size() <= capacity(), "static_vector: insufficient capacity.");
            vx_assert(begin() <= position, "static_vector: invalid iterator.");
            vx_assert(position <= end(), "static_vector: invalid iterator.");

            const auto pos = data() + (position - cbegin());
            const auto end = data() + size();

            // move [pos, end] -> [pos + n, end + n]
            if (pos + n < end) /* move backward */ {
                auto src = end;
                auto dst = end + n;
                while (src != pos) { construct_at(--dst, std::move(*--src)); }
            } else /* move forward */ {
                std::uninitialized_move(pos, end, pos + n);
            }

            // construct at [pos, pos + n]
            std::uninitialized_fill_n(pos, n, x);
            m_size += n;
            return iterator(pos);
        }

        /// @brief Remove the element at position.
        /// @note  Requires `position` is an iterator within the vector.
        /// @return iterator, Iterator after the removed element.
        constexpr iterator erase(const_iterator position) {
            vx_assert(begin() <= position, "static_vector: invalid iterator.");
            vx_assert(position < end(), "static_vector: invalid iterator.");

            const auto pos1 = data() + (position - begin());
            const auto pos2 = pos1 + 1;
            const auto end  = data() + size();
            std::destroy_at(pos1);
            std::uninitialized_move(pos2, end, pos1);

            m_size -= 1;
            return iterator(pos1);
        }

        /// @brief Remove the element in range `first` to `last`.
        /// @note  Requires `first` and `last` are iterators within the vector.
        /// @return iterator, Iterator after the removed elements.
        constexpr iterator erase(const_iterator first, const_iterator last) {
            vx_assert(begin() <= first, "static_vector: invalid iterator.");
            vx_assert(first <= last, "static_vector: invalid iterator.");
            vx_assert(last <= end(), "static_vector: invalid iterator.");
            vx_assert(first < end(), "static_vector: invalid iterator.");

            const auto pos1 = data() + (first - begin());
            const auto pos2 = data() + (last - begin());
            const auto end  = data() + size();
            std::destroy(pos1, pos2);
            std::uninitialized_move(pos2, end, pos1);

            m_size -= (last - first);
            return iterator(pos1);
        }

    private:
        template <typename Arg>
        constexpr static_vector(Arg n, Arg value, std::true_type) noexcept
            : static_vector(static_cast<size_type>(n), static_cast<value_type>(value)) {
            // Consider template parameter as an integral type.
            // Forward to `static_vector(n, value)`.
        }

        template <typename InIter>
        constexpr static_vector(InIter first, InIter last, std::false_type) noexcept
            : m_size(std::distance(first, last)) {
            // Consider template parameter as input iterator.
            vx_assert(m_size <= capacity(), "static_vector: insufficient capacity.");
            std::uninitialized_copy_n(first, m_size, begin());
        }

        template <typename Arg>
        constexpr void assign(Arg n, Arg value, std::true_type) noexcept {
            // Consider template parameter as an integral type.
            // Assign `n` elements of `value`.
            assign(static_cast<size_type>(n), static_cast<value_type>(value));
        }

        template <typename InIter>
        constexpr void assign(InIter first, InIter last, std::false_type) noexcept {
            // Consider template parameter as input iterator.
            const size_type n = std::distance(first, last);
            vx_assert(n <= capacity(), "static_vector: insufficient capacity.");

            if (n < size()) {
                const auto it = std::copy_n(first, n, data());
                std::destroy_n(it, size() - n);
            } else if (n == size()) {
                std::copy_n(first, n, data());
            } else if (n > size()) {
                const auto it = std::copy_n(first, m_size, data());
                std::uninitialized_copy_n(first + size(), n - size(), it);
            }

            m_size = n;
        }

        template <typename Arg>
        constexpr iterator insert(const_iterator position, Arg n, Arg value, std::true_type) {
            // Consider template parameter as an integral type.
            // Forward to `insert(position, n, x)`.
            return insert(position, static_cast<size_type>(n), static_cast<value_type>(value));
        }

        template <typename InIter>
        constexpr iterator insert(const_iterator position, InIter first, InIter last, std::false_type) {
            // Consider template parameter as input iterator.
            const auto n   = std::distance(first, last);
            const auto pos = data() + (position - cbegin());
            const auto end = data() + size();

            vx_assert(n + size() <= capacity(), "static_vector: insufficient capacity.");
            vx_assert(begin() <= position, "static_vector: invalid iterator.");
            vx_assert(position <= end(), "static_vector: invalid iterator.");

            // move [pos, end] -> [pos + n, end + n]
            if (pos + n < end) /* move backward */ {
                auto src = end;
                auto dst = end + n;
                while (src != pos) { construct_at(--dst, std::move(*--src)); }
            } else /* move forward */ {
                std::uninitialized_move(pos, end, pos + n);
            }

            // construct at [pos, pos + n]
            std::uninitialized_copy_n(first, n, pos);
            m_size += n;
            return iterator(pos);
        }
    };

    template <typename T, size_t N>
    vx_forceinline bool operator==(static_vector<T, N> const &x, static_vector<T, N> const &y) noexcept {
        return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
    }

    template <typename T, size_t N>
    vx_forceinline bool operator<(static_vector<T, N> const &x, static_vector<T, N> const &y) noexcept {
        return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template <typename T, size_t N>
    vx_forceinline bool operator!=(static_vector<T, N> const &x, static_vector<T, N> const &y) noexcept {
        return !(x == y);
    }

    template <typename T, size_t N>
    vx_forceinline bool operator>(static_vector<T, N> const &x, static_vector<T, N> const &y) noexcept {
        return y < x;
    }

    template <typename T, size_t N>
    vx_forceinline bool operator<=(static_vector<T, N> const &x, static_vector<T, N> const &y) noexcept {
        return !(y < x);
    }

    template <typename T, size_t N>
    vx_forceinline bool operator>=(static_vector<T, N> const &x, static_vector<T, N> const &y) noexcept {
        return !(x < y);
    }

} // namespace vt

namespace std {

    template <typename T, size_t N>
    constexpr void swap(vt::static_vector<T, N> &x, vt::static_vector<T, N> &y) noexcept(noexcept(x.swap(y))) {
        x.swap(y);
    }

} // namespace std
