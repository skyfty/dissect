#ifndef FIXEDARRAY_H
#define FIXEDARRAY_H

template<typename T, int N>
struct FixedArray {
    T d[N];
    FixedArray() {
        Q_ASSERT(N > 0);
        for(int i = 0; i < N;++i)d[i] = i;
    }

    constexpr const T* begin() const {
        return &d[0];
    }

    constexpr const T* end() const {
        return &d[N];
    }

    constexpr T* begin() {
        return &d[0];
    }

    constexpr T* end() {
        return &d[N];
    }
};


#endif // FIXEDARRAY_H
