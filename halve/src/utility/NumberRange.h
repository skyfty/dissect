#ifndef NUMBERRANGE_H
#define NUMBERRANGE_H

#include <iterator>

template<typename T>
struct RangeIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;

    RangeIterator(value_type v) {
        _v = v;
    }

    bool operator==(const RangeIterator& _Right) const noexcept {
        return _v == _Right._v;
    }

    bool operator!=(const RangeIterator& _Right) const noexcept {
        return !(*this == _Right);
    }
    reference operator*() const noexcept {
        return *operator->();
    }

    pointer operator->() const noexcept {
        return &_v;
    }

    RangeIterator& operator++() noexcept {
        ++_v;
        return *this;
    }

    RangeIterator operator++(int) noexcept {
        RangeIterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    RangeIterator& operator--() noexcept {
        --_v;
        return *this;
    }

    RangeIterator operator--(int) noexcept {
        RangeIterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

private:
    value_type _v; // beginning of array
};


template<typename T>
struct RangeConstIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type        = T;
    using difference_type   = ptrdiff_t;
    using pointer           = const T*;
    using reference         = const T&;

    RangeConstIterator(value_type v) {
        _v = v;
    }

    bool operator==(const RangeConstIterator& _Right) const noexcept {
        return _v == _Right._v;
    }
    bool operator!=(const RangeConstIterator& _Right) const noexcept {
        return !(*this == _Right);
    }

    reference operator*() const noexcept {
        return *operator->();
    }

    pointer operator->() const noexcept {
        return &_v;
    }

    RangeConstIterator& operator++() noexcept {
        ++_v;
        return *this;
    }

    RangeConstIterator operator++(int) noexcept {
        RangeIterator _Tmp = *this;
        ++*this;
        return _Tmp;
    }

    RangeConstIterator& operator--() noexcept {
        --_v;
        return *this;
    }

    RangeConstIterator operator--(int) noexcept {
        RangeIterator _Tmp = *this;
        --*this;
        return _Tmp;
    }

private:
    value_type _v; // beginning of array
};


struct NumberRange {
public:
    using value_type      = int;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using pointer         = int*;
    using const_pointer   = const int*;
    using reference       = int&;
    using const_reference = const int&;
    using iterator       = RangeIterator<value_type>;
    using const_iterator = RangeConstIterator<value_type>;

public:
    NumberRange(int b, int e):m_b(b), m_e(e){}

    iterator begin() noexcept {
        return iterator(m_b);
    }

    iterator end() noexcept {
        return iterator(m_e);
    }

    const_iterator begin() const noexcept {
        return const_iterator(m_b);
    }

    const_iterator end()  const noexcept {
        return const_iterator(m_e);
    }
private:
    int m_b = 0;
    int m_e = 0;
};

#endif // NUMBERRANGE_H
