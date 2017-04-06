/*  Copyright (C) 2017  Povilas Kanapickas <povilas@radix.lt>

    Distributed under the Boost Software License, Version 1.0.
        (See accompanying file LICENSE_1_0.txt or copy at
            http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef TIDY_TIDYSTRING_H
#define TIDY_TIDYSTRING_H

#include <cstring>
#include <algorithm>
#include <initializer_list>
#include <iosfwd>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "tidyvector.h"

namespace tidy {

/*  A string class that does not leak information to memory. That is, upon
    descruction, no traces of the contents of the string can be found in memory
    regardless of operations that are applied to the string.

    The class has not been fully optimized.
*/
template<
    class CharT,
    class Traits = std::char_traits<CharT>,
    class Allocator = std::allocator<CharT>
> class basic_string {
public:
    typedef Traits traits_type;
    typedef typename Traits::char_type value_type;
    typedef Allocator allocator_type;
    typedef typename std::allocator_traits<Allocator>::size_type size_type;
    typedef typename std::allocator_traits<Allocator>::difference_type difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef typename std::allocator_traits<Allocator>::pointer pointer;
    typedef typename std::allocator_traits<Allocator>::const_pointer const_pointer;

    using container_type = vector<CharT, Allocator>;
    using stdstring_type = std::basic_string<CharT, Traits, Allocator>;

    using iterator = typename container_type::iterator;
    using const_iterator = typename container_type::const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // constructor
    basic_string()
    {
        data_.push_back(value_type());
    }

    explicit basic_string(const Allocator& alloc) : data_(alloc)
    {
        data_.push_back(value_type());
    }

    basic_string(size_type count, CharT ch,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        assign(count, ch);
    }

    basic_string(const basic_string& other,
                 size_type pos,
                 size_type count = npos,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        assign(other, pos, count);
    }

    basic_string(const stdstring_type& other,
                 size_type pos,
                 size_type count = npos,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        assign(other, pos, count);
    }

    basic_string(const CharT* s,
                 size_type count,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        // vector(size_type, allocator_type) is since C++14
        assign(s, count);
    }

    basic_string(const CharT* s,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        assign(s);
    }

    template<class InputIt>
    basic_string(InputIt first, InputIt last,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        assign(first, last);
    }

    basic_string(const basic_string& other) = default;

    basic_string(const basic_string& other, const Allocator& alloc) : data_(alloc)
    {
        assign(other);
    }

    basic_string(basic_string&& other) : data_(std::move(other.data_))
    {
        other.data_.push_back(value_type());
    }

    basic_string(std::initializer_list<CharT> init,
                 const Allocator& alloc = Allocator()) : data_(alloc)
    {
        assign(init);
    }

    ~basic_string() {}

    basic_string& operator=(const stdstring_type& str) { return assign(str); }
    basic_string& operator=(const basic_string& str) { return assign(str); }
    basic_string& operator=(const CharT* s) { return assign(s); }
    basic_string& operator=(CharT ch) { clear(); push_back(ch); return *this; }
    basic_string& operator=(basic_string&& other)
    {
        data_ = std::move(other.data_);
        other.data_.push_back(value_type());
        return *this;
    }

    basic_string& operator=(std::initializer_list<CharT> ilist) { return assign(ilist); }

    basic_string& assign(size_type count, const CharT& ch)
    {
        data_.assign(count, ch);
        data_.push_back(value_type());
        return *this;
    }

    basic_string& assign(const basic_string& str)
    {
        data_ = str.data_;
        return *this;
    }

    basic_string& assign(const stdstring_type& str)
    {
        return assign(str.data(), str.size());
    }

    basic_string& assign(const basic_string& str,
                         size_type pos,
                         size_type count = npos)
    {
        size_type size = str.size();
        if (pos > size)
            throw std::out_of_range("pos > size");
        return assign(str.data() + pos, std::min(count, size - pos));
    }

    basic_string& assign(const stdstring_type& str,
                         size_type pos,
                         size_type count = npos)
    {
        size_type size = str.size();
        if (pos > size)
            throw std::out_of_range("pos > size");
        return assign(str.data() + pos, std::min(count, size - pos));
    }

    basic_string& assign(const CharT* s, size_type count)
    {
        data_.resize(count + 1);
        traits_type::move(data_.data(), s, count);
        traits_type::assign(data_[count], value_type());
        return *this;
    }

    basic_string& assign(const CharT* s)
    {
        return assign(s, traits_type::length(s));
    }

    template<class InputIt>
    typename std::enable_if<
        detail::is_input_iterator<InputIt>::value,
        basic_string&
    >::type
        assign(InputIt first, InputIt last)
    {
        data_.assign(first, last);
        data_.push_back(value_type());
        return *this;
    }

    basic_string& assign(std::initializer_list<CharT> ilist)
    {
        return assign(ilist.begin(), ilist.end());
    }

    allocator_type get_allocator() const { return data_.get_allocator(); }

    reference at(size_type n)
    {
        if (n >= size())
            throw std::out_of_range("n >= size()");
        return data_[n];
    }

    const_reference at(size_type n) const
    {
        if (n >= size())
            throw std::out_of_range("n >= size()");
        return data_[n];
    }

    reference       operator[](size_type n) { return data_[n]; }
    const_reference operator[](size_type n) const { return data_[n]; }

    CharT& front() { return data_.front(); }
    const CharT& front() const { return data_.front(); }
    CharT& back() { return data_[data_.size() - 2]; }
    const CharT& back() const { return data_[data_.size() - 2]; }

    const CharT* data() const { return data_.data(); }
    const CharT* c_str() const { return data_.data(); }

    iterator begin() { return data_.begin(); }
    const_iterator begin() const { return data_.begin(); }
    iterator end() { return --(data_.end()); }
    const_iterator end() const { return --(data_.end()); }

    reverse_iterator       rbegin() { return ++(data_.rbegin()); }
    const_reverse_iterator rbegin() const { return ++(data_.rbegin()); }
    reverse_iterator       rend() { return data_.rend(); }
    const_reverse_iterator rend() const { return data_.rend(); }

    const_iterator         cbegin() const { return begin(); }
    const_iterator         cend() const { return end(); }
    const_reverse_iterator crbegin() const { return rbegin(); }
    const_reverse_iterator crend() const { return rend(); }

    // capacity
    bool empty() const { return data_.size() <= 1; }
    size_type size() const { return data_.size() - 1; }
    size_type length() const { return size(); }
    size_type max_size() const { return data_.max_size() - 1; }
    void reserve(size_type new_cap = 0)
    {
        if (new_cap > max_size())
            throw std::length_error("new_cap > max_size()");
        data_.reserve(new_cap + 1);
    }

    size_type capacity() const
    {
        size_type cap = data_.capacity();
        if (cap > 0)
            cap--;
        return cap;
    }

    void shrink_to_fit() { data_.shrink_to_fit(); }
    void clear() { data_.clear(); data_.push_back(value_type()); }

    basic_string& insert(size_type index, size_type count, CharT ch)
    {
        if (index > size())
            throw std::out_of_range("index > size()");
        insert(begin() + index, count, ch);
        return *this;
    }

    basic_string& insert(size_type index, const CharT* s)
    {
        return insert(index, s, traits_type::length(s));
    }

    basic_string& insert(size_type index, const CharT* s, size_type count)
    {
        if (index > size())
            throw std::out_of_range("index > size()");
        data_.insert(begin() + index, s, s+count);
        return *this;
    }

    basic_string& insert(size_type index, const basic_string& str)
    {
        return insert(index, str.data(), str.size());
    }

    basic_string& insert(size_type index, const stdstring_type& str)
    {
        return insert(index, str.data(), str.size());
    }

    basic_string& insert(size_type index, const basic_string& str,
                         size_type index_str, size_type count = npos)
    {
        size_type size = str.size();
        if (index_str > size)
            throw std::out_of_range("index_str > size");
        return insert(index, str.data() + index_str, std::min(count, size - index_str));
    }

    iterator insert(const_iterator pos, CharT ch)
    {
        return data_.insert(pos, ch);
    }

    iterator insert(const_iterator pos, size_type count, CharT ch)
    {
        return data_.insert(pos, count, ch);
    }

    template<class InputIt>
    typename std::enable_if<
        detail::is_input_iterator<InputIt>::value,
        iterator
    >::type
        insert(const_iterator pos, InputIt first, InputIt last)
    {
        return data_.insert(pos, first, last);
    }

    iterator insert(const_iterator pos, std::initializer_list<CharT> ilist)
    {
        return data_.insert(pos, ilist);
    }

    basic_string& erase(size_type index = 0, size_type count = npos)
    {
        if (index > size())
            throw std::out_of_range("index > size");
        count = std::min(count, size() - index);
        erase(begin() + index, begin() + index + count);
        return *this;
    }

    iterator erase(const_iterator pos)
    {
        return data_.erase(pos);
    }

    iterator erase(const_iterator first, const_iterator last)
    {
        return data_.erase(first, last);
    }

    void push_back(CharT ch)
    {
        insert(end(), ch);
    }

    void pop_back()
    {
        data_.pop_back();
        if (data_.size() == 1)
            data_.pop_back();
        else
            data_.back() = value_type();
    }

    void resize(size_type count)
    {
        resize(count, value_type());
    }

    void resize(size_type count, const value_type& value)
    {
        if (count > max_size())
            throw std::length_error("count >= max_size()");
        reserve(count);
        data_.pop_back();
        data_.resize(count, value);
        data_.push_back(value_type());
    }

    basic_string& append(size_type count, CharT ch)
    {
        resize(size() + count, ch);
        return *this;
    }

    basic_string& append(const basic_string& str)
    {
        return append(str.begin(), str.end());
    }

    basic_string& append(const stdstring_type& str)
    {
        return append(str.begin(), str.end());
    }

    basic_string& append(const basic_string& str,
                         size_type pos,
                         size_type count = npos)
    {
        size_type size2 = str.size();
        if (pos > size2)
            throw std::out_of_range("index_str > size");
        return append(str.data() + pos, std::min(count, size2 - pos));
    }

    basic_string& append(const stdstring_type& str,
                         size_type pos,
                         size_type count = npos)
    {
        size_type size2 = str.size();
        if (pos > size2)
            throw std::out_of_range("index_str > size");
        return append(str.data() + pos, std::min(count, size2 - pos));
    }

    basic_string& append(const CharT* s, size_type count)
    {
        size_type old_size = size();
        resize(size() + count, value_type());
        traits_type::move(data_.data() + old_size, s, count);
        return *this;
    }

    basic_string& append(const CharT* s)
    {
        return append(s, traits_type::length(s));
    }

    template<class InputIt>
    typename std::enable_if<
        detail::is_input_iterator<InputIt>::value && !detail::is_forward_iterator<InputIt>::value,
        basic_string&
    >::type
        append(InputIt first, InputIt last)
    {
        for (; first != last; ++first)
            push_back(*first);
        return *this;
    }

    template<class ForwardIt>
    typename std::enable_if<
        detail::is_forward_iterator<ForwardIt>::value,
        basic_string&
    >::type
        append(ForwardIt first, ForwardIt last)
    {
        reserve(size() + std::distance(first, last));
        data_.insert(end(), first, last);
        return *this;
    }

    basic_string& append(std::initializer_list<CharT> ilist)
    {
        return append(ilist.begin(), ilist.end());
    }

    basic_string& operator+=(const basic_string& str) { return append(str); }
    basic_string& operator+=(const stdstring_type& str) { return append(str); }
    basic_string& operator+=(CharT ch) { push_back(ch); return *this; }
    basic_string& operator+=(const CharT* s) { return append(s); }
    basic_string& operator+=(std::initializer_list<CharT> ilist) { return append(ilist); }

    int compare(const basic_string& str) const
    {
        return compare(0, size(), str.data(), str.size());
    }
    int compare(const stdstring_type& str) const
    {
        return compare(0, size(), str.data(), str.size());
    }

    int compare(size_type pos1, size_type count1,
                const basic_string& str) const
    {
        return compare(pos1, count1, str.data(), str.size());
    }
    int compare(size_type pos1, size_type count1,
                const stdstring_type& str) const
    {
        return compare(pos1, count1, str.data(), str.size());
    }

    int compare(size_type pos1, size_type count1,
                const basic_string& str,
                size_type pos2, size_type count2 = npos) const
    {
        size_type size2 = str.size();
        if (pos2 > size2)
            throw std::out_of_range("pos2 > size2");
        return compare(pos1, count1, str.data() + pos2,
                       std::min(count2, size2 - pos2));
    }
    int compare(size_type pos1, size_type count1,
                const stdstring_type& str,
                size_type pos2, size_type count2 = npos) const
    {
        size_type size2 = str.size();
        if (pos2 > size2)
            throw std::out_of_range("pos2 > size2");
        return compare(pos1, count1, str.data() + pos2,
                       std::min(count2, size2 - pos2));
    }

    int compare(const CharT* s) const
    {
        return compare(0, size(), s, traits_type::length(s));
    }

    int compare(size_type pos1, size_type count1,
                const CharT* s) const
    {
        return compare(pos1, count1, s, traits_type::length(s));
    }

    int compare(size_type pos1, size_type count1,
                const CharT* s, size_type count2) const
    {
        size_type size1 = size();
        if (pos1 > size1)
            throw std::out_of_range("pos2 > size2");
        count1 = std::min(count1, size1 - pos1);
        size_type compare_len = std::min(count1, count2);

        int ret = traits_type::compare(data() + pos1, s, compare_len);
        if (ret == 0) {
            if (count1 < count2)
                return -1;
            if (count1 > count2)
                return 1;
        }
        return ret;
    }

    /*
    basic_string& replace(size_type pos, size_type count,
                              const basic_string& str);
    basic_string& replace(const_iterator first, const_iterator last,
                              const basic_string& str);
    basic_string& replace(size_type pos, size_type count,
                              const basic_string& str,
                              size_type pos2, size_type count2 = npos);
    template<class InputIt>
    basic_string& replace(const_iterator first, const_iterator last,
                              InputIt first2, InputIt last2);

    basic_string& replace(size_type pos, size_type count,
                              const CharT* cstr, size_type count2);

    basic_string& replace(const_iterator first, const_iterator last,
                              const CharT* cstr, size_type count2);

    basic_string& replace(size_type pos, size_type count,
                              const CharT* cstr);

    basic_string& replace(const_iterator first, const_iterator last,
                              const CharT* cstr);

    basic_string& replace(size_type pos, size_type count,
                              size_type count2, CharT ch);

    basic_string& replace(const_iterator first, const_iterator last,
                              size_type count2, CharT ch);

    basic_string& replace(const_iterator first, const_iterator last,
                              std::initializer_list<CharT> ilist);
    */
    basic_string substr(size_type pos = 0,
                            size_type count = npos) const
    {
        basic_string str(get_allocator());
        if (pos > size())
            throw std::out_of_range("pos > size");
        str.assign(data() + pos, std::min(count, size() - pos));
        return str;
    }

    size_type copy(CharT* dest,
                   size_type count,
                   size_type pos = 0) const
    {
        if (pos > size())
            throw std::out_of_range("pos > size");
        count = std::min(count, size() - pos);
        traits_type::move(dest, data() + pos, count);
        return count;
    }

    void swap(basic_string& other)
    {
        data_.swap(other.data_);
    }

    // search
    size_type find(const stdstring_type& str, size_type pos = 0) const
    {
        return find(str.data(), pos, str.size());
    }
    size_type find(const basic_string& str, size_type pos = 0) const
    {
        return find(str.data(), pos, str.size());
    }
    size_type find(const CharT* s, size_type pos, size_type count) const
    {
        if (pos > size() || size() - pos < count)
            return npos;
        if (count == 0)
            return pos;
        auto ret = std::search(begin() + pos, end(), s, s + count, traits_type::eq);
        if (ret == end())
            return npos;
        return std::distance(begin(), ret);
    }
    size_type find(const CharT* s, size_type pos = 0) const
    {
        return find(s, pos, traits_type::length(s));
    }
    size_type find(CharT ch, size_type pos = 0) const
    {
        if (pos > size())
            return npos;
        auto ret = std::find(begin() + pos, end(), ch);
        if (ret == end())
            return npos;
        return std::distance(begin(), ret);
    }

    size_type rfind(const stdstring_type& str, size_type pos = npos) const
    {
        return rfind(str.data(), pos, str.size());
    }
    size_type rfind(const basic_string& str, size_type pos = npos) const
    {
        return rfind(str.data(), pos, str.size());
    }
    size_type rfind(const CharT* s, size_type pos, size_type count) const
    {
        if (pos < size() && count < size() - pos)
            pos += count;
        else
            pos = size();

        auto ret = std::find_end(begin(), begin() + pos,
                                 s, s + count, traits_type::eq);
        if (count > 0 && ret == begin() + pos)
            return npos;
        return std::distance(begin(), ret);
    }
    size_type rfind(const CharT* s, size_type pos = npos) const
    {
        return rfind(s, pos, traits_type::length(s));
    }
    size_type rfind(CharT ch, size_type pos = npos) const
    {
        if (empty())
            return npos;
        if (pos < size())
            pos++;
        else
            pos = size();
        for (auto it = begin() + pos; it != begin();) {
            --it;
            if (traits_type::eq(*it, ch))
                return std::distance(begin(), it);
        }
        return npos;
    }

    size_type find_first_of(const stdstring_type& str, size_type pos = 0) const
    {
        return find_first_of(str.data(), pos, str.size());
    }
    size_type find_first_of(const basic_string& str, size_type pos = 0) const
    {
        return find_first_of(str.data(), pos, str.size());
    }
    size_type find_first_of(const CharT* s, size_type pos, size_type count) const
    {
        if (pos >= size() || count == 0)
            return npos;
        for (auto it1 = begin() + pos; it1 != end(); ++it1) {
            for (const CharT* it2 = s; it2 < s + count; ++it2) {
                if (traits_type::eq(*it1, *it2))
                    return std::distance(begin(), it1);
            }
        }
        return npos;
    }
    size_type find_first_of(const CharT* s, size_type pos = 0) const
    {
        return find_first_of(s, pos, traits_type::length(s));
    }
    size_type find_first_of(CharT ch, size_type pos = 0) const
    {
        return find(ch, pos);
    }

    size_type find_first_not_of(const stdstring_type& str, size_type pos = 0) const
    {
        return find_first_not_of(str.data(), pos, str.size());
    }
    size_type find_first_not_of(const basic_string& str, size_type pos = 0) const
    {
        return find_first_not_of(str.data(), pos, str.size());
    }
    size_type find_first_not_of(const CharT* s, size_type pos, size_type count) const
    {
        if (pos >= size())
            return npos;
        for (auto it = begin() + pos; it != end(); ++it) {
            if (traits_type::find(s, count, *it) == nullptr)
                return std::distance(begin(), it);
        }
        return npos;
    }
    size_type find_first_not_of(const CharT* s, size_type pos = 0) const
    {
        return find_first_not_of(s, pos, traits_type::length(s));
    }
    size_type find_first_not_of(CharT ch, size_type pos = 0) const
    {
        if (pos >= size())
            return npos;
        for (auto it = begin() + pos; it != end(); ++it) {
            if (!traits_type::eq(*it, ch))
                return std::distance(begin(), it);
        }
        return npos;
    }

    size_type find_last_of(const stdstring_type& str, size_type pos = npos) const
    {
        return find_last_of(str.data(), pos, str.size());
    }
    size_type find_last_of(const basic_string& str, size_type pos = npos) const
    {
        return find_last_of(str.data(), pos, str.size());
    }
    size_type find_last_of(const CharT* s, size_type pos, size_type count) const
    {
        if (count == 0)
            return npos;
        if (pos < size())
            pos++;
        else
            pos = size();
        for (auto it = begin() + pos; it != begin(); ) {
            --it;
            if (traits_type::find(s, count, *it) != nullptr)
                return std::distance(begin(), it);
        }
        return npos;
    }
    size_type find_last_of(const CharT* s, size_type pos = npos) const
    {
        return find_last_of(s, pos, traits_type::length(s));
    }
    size_type find_last_of(CharT ch, size_type pos = npos) const
    {
        return rfind(ch, pos);
    }

    size_type find_last_not_of(const stdstring_type& str, size_type pos = npos) const
    {
        return find_last_not_of(str.data(), pos, str.size());
    }
    size_type find_last_not_of(const basic_string& str, size_type pos = npos) const
    {
        return find_last_not_of(str.data(), pos, str.size());
    }
    size_type find_last_not_of(const CharT* s, size_type pos, size_type count) const
    {
        if (pos < size())
            pos++;
        else
            pos = size();
        for (auto it = begin() + pos; it != begin(); ) {
            --it;
            if (traits_type::find(s, count, *it) == nullptr)
                return std::distance(begin(), it);
        }
        return npos;
    }
    size_type find_last_not_of(const CharT* s, size_type pos = npos) const
    {
        return find_last_not_of(s, pos, traits_type::length(s));
    }
    size_type find_last_not_of(CharT ch, size_type pos = npos) const
    {
        if (pos < size())
            pos++;
        else
            pos = size();
        for (auto it = begin() + pos; it != begin(); ) {
            --it;
            if (!traits_type::eq(ch, *it))
                return std::distance(begin(), it);
        }
        return npos;
    }

    bool __invariants() const
    {
        return !data_.empty() && data_.__invariants();
    }

    static const size_type npos = -1;
private:
    container_type data_;
};

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;


template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(const basic_string<CharT, Traits, Alloc>& lhs,
              const basic_string<CharT, Traits, Alloc>& rhs)
{
    basic_string<CharT, Traits, Alloc> str = lhs;
    str.append(rhs);
    return str;
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(const CharT* lhs,
              const basic_string<CharT, Traits, Alloc>& rhs)
{
    basic_string<CharT, Traits, Alloc> str;
    str.assign(lhs);
    str.append(rhs);
    return str;
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(CharT lhs,
              const basic_string<CharT, Traits, Alloc>& rhs)
{
    basic_string<CharT, Traits, Alloc> str;
    str.push_back(lhs);
    str.append(rhs);
    return str;
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(const basic_string<CharT, Traits, Alloc>& lhs,
              const CharT* rhs)
{
    basic_string<CharT, Traits, Alloc> str = lhs;
    str.append(rhs);
    return str;
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(const basic_string<CharT, Traits, Alloc>& lhs,
              CharT rhs)
{
    basic_string<CharT, Traits, Alloc> str = lhs;
    str.push_back(rhs);
    return str;
}

// TODO
template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(basic_string<CharT, Traits, Alloc>&& lhs,
              const basic_string<CharT, Traits, Alloc>& rhs)
{
    lhs.append(rhs);
    return std::move(lhs);
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(const basic_string<CharT, Traits, Alloc>& lhs,
              basic_string<CharT, Traits, Alloc>&& rhs)
{
    rhs.insert(0, lhs);
    return std::move(rhs);
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(basic_string<CharT, Traits, Alloc>&& lhs,
              basic_string<CharT, Traits, Alloc>&& rhs)
{
    lhs.append(rhs);
    return std::move(lhs);
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(const CharT* lhs,
              basic_string<CharT, Traits, Alloc>&& rhs)
{
    rhs.insert(0, lhs);
    return std::move(rhs);
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(CharT lhs,
              basic_string<CharT, Traits, Alloc>&& rhs)
{
    rhs.insert(rhs.begin(), lhs);
    return std::move(rhs);
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(basic_string<CharT, Traits, Alloc>&& lhs,
              const CharT* rhs)
{
    lhs.append(rhs);
    return std::move(lhs);
}

template<class CharT, class Traits, class Alloc>
inline basic_string<CharT, Traits, Alloc>
    operator+(basic_string<CharT, Traits, Alloc>&& lhs,
              CharT rhs)
{
    lhs.push_back(rhs);
    return std::move(lhs);
}

// comparisons between tidy::basic_string
template<class CharT, class Traits, class Alloc>
inline bool operator==(const basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) == 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) != 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<(const basic_string<CharT, Traits, Alloc>& lhs,
                      const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) < 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<=(const basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) <= 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>(const basic_string<CharT, Traits, Alloc>& lhs,
                      const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) > 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>=(const basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) >= 0;
}

// comparisons between tidy::basic_string and std::basic_string
template<class CharT, class Traits, class Alloc>
inline bool operator==(const basic_string<CharT, Traits, Alloc>& lhs,
                       const std::basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) == 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs,
                       const std::basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) != 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<(const basic_string<CharT, Traits, Alloc>& lhs,
                      const std::basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) < 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<=(const basic_string<CharT, Traits, Alloc>& lhs,
                       const std::basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) <= 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>(const basic_string<CharT, Traits, Alloc>& lhs,
                      const std::basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) > 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>=(const basic_string<CharT, Traits, Alloc>& lhs,
                       const std::basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) >= 0;
}

// comparisons between std::basic_string and tidy::basic_string
template<class CharT, class Traits, class Alloc>
inline bool operator==(const std::basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) == 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator!=(const std::basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) != 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<(const std::basic_string<CharT, Traits, Alloc>& lhs,
                      const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) < 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<=(const std::basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) <= 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>(const std::basic_string<CharT, Traits, Alloc>& lhs,
                      const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) > 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>=(const std::basic_string<CharT, Traits, Alloc>& lhs,
                       const basic_string<CharT, Traits, Alloc>& rhs)
{
    return lhs.compare(rhs) >= 0;
}

// comparisons between tidy::basic_string and nul-terminated strings
template<class CharT, class Traits, class Alloc>
inline bool operator==(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return rhs.compare(lhs) == 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator==(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
{
    return lhs.compare(rhs) == 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator!=(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return rhs.compare(lhs) != 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator!=(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
{
    return lhs.compare(rhs) != 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return rhs.compare(lhs) > 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<(const basic_string<CharT, Traits, Alloc>& lhs,  const CharT* rhs)
{
    return lhs.compare(rhs) < 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<=(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return rhs.compare(lhs) >= 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator<=(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
{
    return lhs.compare(rhs) <= 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return rhs.compare(lhs) < 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
{
    return lhs.compare(rhs) > 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>=(const CharT* lhs, const basic_string<CharT, Traits, Alloc>& rhs)
{
    return rhs.compare(lhs) <= 0;
}

template<class CharT, class Traits, class Alloc>
inline bool operator>=(const basic_string<CharT, Traits, Alloc>& lhs, const CharT* rhs)
{
    return lhs.compare(rhs) >= 0;
}

template<class T, class Traits, class Alloc>
inline void swap(basic_string<T, Traits, Alloc>& lhs, basic_string<T, Traits, Alloc>& rhs)
{
    lhs.swap(rhs);
}

template<class CharT, class Traits, class Alloc>
inline std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os,
               const basic_string<CharT, Traits, Alloc>& str)
{
    // Use basic_string functionality because it might handle nul characters
    // differently
    std::basic_string<CharT, Traits, Alloc> std_str;
    std_str.assign(str.data(), str.size());
    os << std_str;
    return os;
}

template<class CharT, class Traits, class Alloc>
inline std::basic_istream<CharT, Traits>&
    operator>>(std::basic_istream<CharT, Traits>& is,
               basic_string<CharT, Traits, Alloc>& str)
{
    // Use basic_string functionality because it might handle nul characters
    // differently
    std::basic_string<CharT, Traits, Alloc> std_str;
    is >> std_str;
    str.assign(std_str.data(), std_str.size());
    return is;
}

} // namespace tidy

#endif
