/*  Copyright (C) 2017  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef TIDY_TIDYVECTOR_H
#define TIDY_TIDYVECTOR_H

#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <vector>
#include <type_traits>

namespace tidy {
namespace detail {

template<class T>
struct has_iterator_category {
    struct Char2 { char value[2]; };
    template<class U> static char fun(...);
    template<class U> static Char2 fun(typename U::iterator_category*);
public:
    static const bool value = sizeof(fun<T>(0)) > 1;
};

template<class T, class Tag,
         bool = has_iterator_category<std::iterator_traits<T>>::value>
struct is_iterator_tag :
        public std::integral_constant<
            bool, std::is_base_of<Tag, typename std::iterator_traits<T>::iterator_category>::value
        >
{};

template<class T, class Tag>
struct is_iterator_tag<T, Tag, false> : public std::false_type {};

template<class T>
struct is_input_iterator : public is_iterator_tag<T, std::input_iterator_tag> {};
template<class T>
struct is_forward_iterator : public is_iterator_tag<T, std::forward_iterator_tag> {};

static void secure_delete(const void* ptr, unsigned size)
{
    // This function might be the last thing that uses the data in question
    // so we want to fool dead-code optimizations of the compiler to not
    // optimize this out.
    void* nonconst_ptr = const_cast<void*>(ptr);
    void** volatile vol_ptr = &nonconst_ptr;
    std::memset(*vol_ptr, 0, size);
}

template<class T>
static void secure_delete_range(T* begin, T* end)
{
    if (begin >= end)
        return;
    secure_delete(begin, (end - begin) * sizeof(T));
}

template<class T, class Alloc>
static void secure_delete_vector(std::vector<T,Alloc>& v)
{
    using value_type = typename std::vector<T,Alloc>::value_type;
    if (v.size() > 0)
        secure_delete(v.data(), v.size() * sizeof(value_type));
}

template<class T, class Alloc>
static void secure_delete_vector_capacity(std::vector<T,Alloc>& v)
{
    using value_type = typename std::vector<T,Alloc>::value_type;
    if (v.size() > 0)
        secure_delete(v.data(), v.capacity() * sizeof(value_type));
}

} // namespace detail

/*  A vector class that does not leak information to memory. That is, upon
    descruction, no traces of the contents of the vector can be found in memory
    regardless of operations that are applied to the vector.

    Only trivial types are supported. In addition to that, the class depends
    on std::vector not doing any deallocations on move-assignment or swap.
*/
template<class T, class Allocator = std::allocator<T>>
class vector {
public:
    static_assert(std::is_trivial<T>::value,
                  "Only trivial types are supported");

    using container_type = std::vector<T, Allocator>;

    typedef T value_type;
    typedef Allocator allocator_type;
    typedef typename container_type::size_type size_type;
    typedef typename container_type::difference_type difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef typename std::allocator_traits<Allocator>::pointer pointer;
    typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    vector() {}

    explicit vector(const Allocator& alloc) : data_(alloc) {}

    vector(size_type count, const T& value) : data_(count, value) {}

    vector(size_type count, const T& value,
           const Allocator& alloc) :
        data_(count, value, alloc)
    {}

    explicit vector(size_type n) : data_(n) {}
    explicit vector(size_type n, const Allocator& alloc) : data_(n, alloc) {}

    template<class InputIt>
    vector(InputIt first, InputIt last) : data_(first, last) {}

    template<class InputIt>
    vector(InputIt first, InputIt last,
           const Allocator& alloc) : data_(first, last, alloc)
    {}

    vector(const vector& other) : data_(other.data_) {}
    vector(const vector& other, const Allocator& alloc) : data_(other.data_, alloc) {}

    vector(vector&& other) : data_(std::move(other.data_))
    {
        detail::secure_delete_vector_capacity(other.data_);
    }

    vector(vector&& other, const Allocator& alloc) : data_(std::move(other.data_), alloc)
    {
        detail::secure_delete_vector_capacity(other.data_);
    }

    vector(std::initializer_list<T> init) : data_(init) {}

    vector(std::initializer_list<T> init,
           const Allocator& alloc) :
        data_(init, alloc)
    {}

    ~vector()
    {
        detail::secure_delete_vector(data_);
    }

    vector& operator=(const vector& other)
    {
        if (this != &other)
        {
            detail::secure_delete_vector(data_);
            data_ = other.data_;
        }
        return *this;
    }

    vector& operator=(vector&& other)
    {
        detail::secure_delete_vector(data_);
        data_ = std::move(other.data_);
        detail::secure_delete_vector_capacity(other.data_);
        return *this;
    }

    vector& operator=(std::initializer_list<T> ilist)
    {
        assign(ilist);
        return *this;
    }

    void assign(size_type count, const value_type& value)
    {
        clear();
        resize(count, value);
    }

    template<class InputIt>
    typename std::enable_if<
        detail::is_input_iterator<InputIt>::value && !detail::is_forward_iterator<InputIt>::value,
        void
    >::type
        assign(InputIt first, InputIt last)
    {
        clear();
        for (; first != last; ++first)
            push_back(*first);
    }

    template<class ForwardIt>
    typename std::enable_if<
        detail::is_forward_iterator<ForwardIt>::value,
        void
    >::type
        assign(ForwardIt first, ForwardIt last)
    {
        clear();
        reserve(std::distance(first, last));
        for (; first != last; ++first)
            push_back(*first);
    }

    void assign(std::initializer_list<T> ilist)
    {
        assign(ilist.begin(), ilist.end());
    }

    allocator_type get_allocator() const { return data_.get_allocator(); }

    // element access
    reference       at(size_type n) { return data_.at(n); }
    const_reference at(size_type n) const { return data_.at(n); }
    reference       operator[](size_type n) { return data_[n]; }
    const_reference operator[](size_type n) const { return data_[n]; }

    reference       front() { return data_.front(); }
    const_reference front() const { return data_.front(); }
    reference       back() { return data_.back(); }
    const_reference back() const { return data_.back(); }
    value_type*       data() { return data_.data(); }
    const value_type* data() const { return data_.data(); }

    // iterators
    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    iterator end() { return data_.end(); }
    const_iterator end() const { return data_.end(); }

    reverse_iterator       rbegin() { return data_.rbegin(); }
    const_reverse_iterator rbegin() const { return data_.rbegin(); }
    reverse_iterator       rend() { return data_.rend(); }
    const_reverse_iterator rend() const { return data_.rend(); }

    const_iterator         cbegin() const { return data_.cbegin(); }
    const_iterator         cend() const { return data_.cend(); }
    const_reverse_iterator crbegin() const { return data_.crbegin(); }
    const_reverse_iterator crend() const { return data_.crend(); }

    // capacity
    bool empty() const { return data_.empty(); }
    size_type size() const { return data_.size(); }
    size_type max_size() const { return data_.max_size(); }
    void reserve(size_type n)
    {
        if (n <= capacity())
            return;
        reallocate(n);
    }

    size_type capacity() const { return data_.capacity(); }

    void shrink_to_fit()
    {
        reallocate(size());
    }

    void clear()
    {
        detail::secure_delete_vector(data_);
        data_.clear();
    }

    iterator insert(const_iterator pos, const T& value)
    {
        difference_type index = std::distance(cbegin(), pos);
        reserve_grow(size() + 1);
        return data_.insert(cbegin() + index, value);
    }

    iterator insert(const_iterator pos, T&& value)
    {
        difference_type index = std::distance(cbegin(), pos);
        reserve_grow(size() + 1);
        return data_.insert(cbegin() + index, std::move(value));
    }

    iterator insert(const_iterator pos, size_type count, const T& value)
    {
        difference_type index = std::distance(cbegin(), pos);
        reserve_grow(size() + count);
        return data_.insert(cbegin() + index, count, value);
    }

    template<class InputIt>
    typename std::enable_if<
        detail::is_input_iterator<InputIt>::value && !detail::is_forward_iterator<InputIt>::value,
        iterator
    >::type
        insert(const_iterator pos, InputIt first, InputIt last)
    {
        size_type old_size = size();
        difference_type index = pos - cbegin();
        for (; first != last; ++first)
            push_back(*first);

        std::rotate(begin() + index,
                    begin() + old_size,
                    end());
        return begin() + index;
    }

    template<class ForwardIt>
    typename std::enable_if<
        detail::is_forward_iterator<ForwardIt>::value,
        iterator
    >::type
        insert(const_iterator pos, ForwardIt first, ForwardIt last)
    {
        difference_type index = pos - cbegin();
        reserve_grow(size() + std::distance(first, last));
        return data_.insert(cbegin() + index, first, last);
    }

    iterator insert(const_iterator pos, std::initializer_list<T> ilist)
    {
        difference_type index = pos - cbegin();
        reserve_grow(size() + std::distance(ilist.begin(), ilist.end()));
        return data_.insert(cbegin() + index, ilist);
    }

    template<class... Args>
    iterator emplace(const_iterator pos, Args&& ... args)
    {
        difference_type index = pos - cbegin();

        if (index == size() || size() == capacity()) {
            value_type temp(std::forward<Args>(args)...);
            reserve_grow(size() + 1);
            return data_.insert(cbegin() + index, std::move(temp));
        }

        reserve_grow(size() + 1);
        return data_.emplace(cbegin() + index, std::move(args...));
    }

    iterator erase(const_iterator pos)
    {
        iterator ret = data_.erase(pos);

        value_type* p_end = data_.data() + data_.size();
        detail::secure_delete_range(p_end, p_end + 1);

        return ret;
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        difference_type count = std::distance(first, last);
        iterator ret = data_.erase(first, last);

        if (count > 0) {
            value_type* p_end = data_.data() + data_.size();
            detail::secure_delete_range(p_end, p_end + count);
        }

        return ret;
    }

    void push_back(const T& value)
    {
        reserve_grow(size() + 1);
        data_.push_back(value);
    }

    void push_back(T&& value)
    {
        reserve_grow(size() + 1);
        data_.push_back(std::move(value));
    }

    template<class... Args>
    void emplace_back(Args&& ... args)
    {
        reserve_grow(size() + 1);
        data_.emplace_back(std::move(args...));
    }

    void pop_back()
    {
        data_.pop_back();
        value_type* p_end = data_.data() + data_.size();
        detail::secure_delete_range(p_end, p_end + 1);
    }

    void resize(size_type count)
    {
        if (count > max_size())
            throw std::length_error("count > max_size()");
        if (count < size()) {
            value_type* new_end = data_.data() + count;
            value_type* old_end = data_.data() + data_.size();
            detail::secure_delete_range(new_end, old_end);
            data_.resize(count);
        } else {
            reserve_grow(count);
            data_.resize(count);
        }
    }

    void resize(size_type count, const value_type& value)
    {
        if (count > max_size())
            throw std::length_error("count > max_size()");
        if (count < size()) {
            value_type* new_end = data_.data() + count;
            value_type* old_end = data_.data() + data_.size();
            detail::secure_delete_range(new_end, old_end);
            data_.resize(count, value);
        } else {
            reserve_grow(count);
            data_.resize(count, value);
        }
    }

    void swap(vector& other)
    {
        data_.swap(other.data_);
    }

    const container_type& container() const { return data_; }

    // dummy function that aids compatibility with libcxx tests
    bool __invariants() const
    {
#ifdef _LIBCPP_VECTOR
        return data_.__invariants();
#else
        return true;
#endif
    }

private:

    // We can't use reserve() when growing container, because it will allocate
    // absolute minimum space.
    void reserve_grow(size_type size)
    {
        if (size <= capacity())
            return;
        reallocate(std::max(2 * capacity(), size));
    }

    void reallocate(size_type new_capacity)
    {
        // support stateful allocators
        reallocate_impl(new_capacity, std::is_empty<allocator_type>());
    }

    void reallocate_impl(size_type new_capacity, std::true_type /*is_empty*/)
    {
        vector temp;
        temp.data_.reserve(new_capacity);
        std::move(begin(), end(), std::back_inserter(temp.data_));
        swap(temp);
    }

    void reallocate_impl(size_type new_capacity, std::false_type /*is_not_empty*/)
    {
        // make sure we preserve the stateful allocator
        vector temp;
        temp.reserve(size());
        std::move(begin(), end(), std::back_inserter(temp.data_));
        clear();
        data_.reserve(new_capacity);
        std::move(temp.begin(), temp.end(), std::back_inserter(data_));
    }

    container_type data_;
};

template<class T, class Alloc>
bool operator==(const vector<T, Alloc>& lhs,
                const vector<T, Alloc>& rhs)
{
    return lhs.container() == rhs.container();
}

template<class T, class Alloc>
bool operator!=(const vector<T, Alloc>& lhs,
                const vector<T, Alloc>& rhs)
{
    return lhs.container() != rhs.container();
}

template<class T, class Alloc>
bool operator<(const vector<T, Alloc>& lhs,
               const vector<T, Alloc>& rhs)
{
    return lhs.container() < rhs.container();
}

template<class T, class Alloc>
bool operator<=(const vector<T, Alloc>& lhs,
                const vector<T, Alloc>& rhs)
{
    return lhs.container() <= rhs.container();
}

template<class T, class Alloc>
bool operator>(const vector<T, Alloc>& lhs,
               const vector<T, Alloc>& rhs)
{
    return lhs.container() > rhs.container();
}

template<class T, class Alloc>
bool operator>=(const vector<T, Alloc>& lhs,
                const vector<T, Alloc>& rhs)
{
    return lhs.container() >= rhs.container();
}

template<class T, class Alloc>
void swap(vector<T, Alloc>& lhs, vector<T, Alloc>& rhs)
{
    lhs.swap(rhs);
}

} // namespace tidy

#endif
