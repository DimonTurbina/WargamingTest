#pragma once
#include <exception>

template<class ElemType = unsigned>
class CircularBuffer {
    ElemType* data;
    unsigned _capacity;
    unsigned _size;
    unsigned _tail;
public:

    class Iterator : public std::iterator<std::random_access_iterator_tag, ElemType> {
    private:
        ElemType* iterator;
    public:
        using difference_type = typename std::iterator<std::random_access_iterator_tag, ElemType>::difference_type;
        Iterator() : iterator(nullptr) {}
        explicit Iterator(ElemType* it) : iterator(it) {}
        Iterator(const Iterator& other) : iterator(other.iterator) {}
        inline Iterator& operator+=(difference_type it) { iterator += it; return *this; }
        inline Iterator& operator-=(difference_type it) { iterator -= it; return *this; }
        inline ElemType* operator->() const { return iterator; }
        inline ElemType& operator*() const { return *iterator; }
        inline ElemType& operator[](difference_type i) const { return iterator[i]; }
        inline Iterator& operator++() { ++iterator; return *this; }
        inline Iterator& operator--() { --iterator; return *this; }
        inline Iterator operator++(ElemType) { Iterator tmp(*this); ++iterator; return tmp; }
        inline Iterator operator--(ElemType) { Iterator tmp(*this); --iterator; return tmp; }
        inline difference_type operator-(const Iterator& it) const { return iterator - it.iterator; }
        inline Iterator operator+(difference_type it) const { return Iterator(iterator + it); }
        inline Iterator operator-(difference_type it) const { return Iterator(iterator - it); }
        friend inline Iterator operator+(difference_type lhs, const Iterator& rhs) { return Iterator(lhs + rhs.iterator); }
        friend inline Iterator operator-(difference_type lhs, const Iterator& rhs) { return Iterator(lhs - rhs.iterator); }
        inline bool operator==(const Iterator& other) const { return iterator == other.iterator; }
        inline bool operator!=(const Iterator& other) const { return iterator != other.iterator; }
        inline bool operator>(const Iterator& other) const { return iterator > other.iterator; }
        inline bool operator<(const Iterator& other) const { return iterator < other.iterator; }
        inline bool operator<=(const Iterator& other) const { return iterator <= other.iterator; }
        inline bool operator>=(const Iterator& other) const { return iterator >= other.iterator; }
    };

    Iterator begin() const {
        return Iterator(data);
    }

    Iterator end() const {
        return Iterator(data + _size);
    }

    ElemType operator[](int index) const {
        if (index < 0 || index > _size - 1) throw (std::out_of_range("Out of range!"));

        return data[index];
    }

    explicit CircularBuffer(unsigned capacity = 1) : _capacity(capacity), _size(0), _tail(0), data(new ElemType[capacity])
    {
        for (size_t i = 0; i < _capacity; ++i) {
            data[i] = 0;
        }
    }

    ~CircularBuffer() {
        delete[] data;
    }

    std::size_t size() const noexcept {
        return _size;
    }

    void addLast(ElemType value) {
        if (_tail >= _capacity) {
            _tail = 0;
        }
        if (_size == _capacity) {
            data[_tail] = value;
        }
        else {
            ElemType* bufferData = new ElemType[_size + 1];
            for (size_t i = 0; i < _size; ++i) {
                bufferData[i] = data[i];
            }
            bufferData[_size] = value;
            delete[] data;
            data = bufferData;
            ++_size;
        }
        _tail++;
    }

    void removeLast() {
        if (_size == 0) throw std::out_of_range("Empty array!");

        ElemType* bufferData = new ElemType[_size - 1];
        for (size_t i = 0; i < _size - 1; ++i) {
            bufferData[i] = data[i];
        }
        delete[] data;
        data = bufferData;
        --_size;
    }

    void addFirst(ElemType value) {
        if (_size == _capacity) {
            ElemType* bufferData = new ElemType[_size];
            bufferData[0] = value;
            for (auto i = 1; i < _size; ++i) {
                bufferData[i] = data[i - 1];
            }
            delete[] data;
            data = bufferData;
        }
        else {
            ElemType* bufferData = new ElemType[_size + 1];
            for (auto i = 1; i < _size + 1; ++i) {
                bufferData[i] = data[i - 1];
            }
            bufferData[0] = value;
            delete[] data;
            data = bufferData;
            ++_size;
        }
    }

    void removeFirst() {
        if (_size == 0) throw std::out_of_range("Empty array!");

        ElemType* bufferData = new ElemType[_size - 1];
        for (size_t i = 0; i < _size; ++i) {
            bufferData[i] = data[i + 1];
        }
        delete[] data;
        data = bufferData;
        --_size;
    }

    void addByIndex(std::size_t index, ElemType value) {
        if (_size == _capacity) {
            data[index] = value;
        }
        else {
            ElemType* bufferData = new ElemType[_size + 1];
            for (auto i = 0; i < index; ++i) {
                bufferData[i] = data[i];
            }
            bufferData[index] = value;
            for (auto i = index + 1; i < _size + 1; ++i) {
                bufferData[i] = data[i - 1];
            }
            delete[] data;
            data = bufferData;
            ++_size;
        }
    }

    void removeByIndex(std::size_t index) {
        if (_size == 0) throw std::out_of_range("Empty array!");

        else {
            ElemType* bufferData = new ElemType[_size - 1];
            for (size_t i = 0; i < index; ++i) {
                bufferData[i] = data[i];
            }
            for (size_t i = index; i < _size; ++i) {
                bufferData[i] = data[i + 1];
            }
            delete[] data;
            data = bufferData;
            --_size;
        }
    }

    ElemType getFirst() const noexcept {
        if (_size == 0) throw std::out_of_range("Empty array!");

        return data[0];
    }

    ElemType getLast() const noexcept {
        if (_size == 0) throw std::out_of_range("Empty array!");

        return data[_size - 1];
    }

    void changeCapacity(unsigned newCapacity) {
        if (newCapacity <= _capacity) throw std::bad_alloc();

        ElemType* bufferData = new ElemType[newCapacity];
        for (size_t i = 0; i < _size; ++i) {
            bufferData[i] = data[i];
        }
        delete[] data;
        data = bufferData;
        _capacity = newCapacity;
        _tail = _size;
    }
};