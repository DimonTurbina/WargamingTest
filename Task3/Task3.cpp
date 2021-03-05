#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

template<typename T>
void SWAP(T& a, T& b){                                                                            
    T tmp = a;                                                                 
    a = b;                                                                     
    b = tmp;  
}

#define MAX(a, b, c) (a >= b) ? (a >= c) ? a : c : (b >= c) ? b : c

template<typename T>
void IntroSort(T* arr, int32_t size);

template<typename T>
void ToHeap(T* arr, int32_t size, int32_t i) {
    int32_t largest = i;
    int32_t l = 2 * i + 1;
    int32_t r = 2 * i + 2;

    if (l < size && arr[l] > arr[largest])
        largest = l;
    if (r < size && arr[r] > arr[largest])
        largest = r;
    if (largest != i) {
        SWAP<T>(arr[i], arr[largest]);

        ToHeap(arr, size, largest);
    }
}

template<typename T>
void HeapSort(T* arr, int32_t size) {
    for (int32_t i = size / 2 - 1; i >= 0; i--)
        ToHeap(arr, size, i);
    for (size_t i = size - 1; i >= 0; i--) {
        SWAP<T>(arr[0], arr[i]);

        ToHeap(arr, i, 0);
    }
}

template<typename T>
int Partition(T* arr, int32_t begin, int32_t end) {
    double pivot_value = arr[begin];
    int i = begin - 1;
    int j = end + 1;
    while (i <= j) {
        while (arr[++i] < pivot_value)
            ;
        while (arr[--j] > pivot_value)
            ;
        if (i < j) {
            SWAP<T>(arr[i], arr[j]);
        }
        else {
            break;
        }
    }
    return j;
}

template<typename T>
void QuickSort(T* arr, int32_t begin, int32_t end) {
    if (begin < end) {
        int32_t pivot = Partition(arr, begin, end);
        QuickSort(arr, begin, pivot);
        QuickSort(arr, pivot + 1, end);
    }
}

template<typename T>
void IntroSort(T* arr, int32_t maxdepth, int32_t start, int32_t end);

template<typename T>
void IntroSort(T* arr, int32_t size) {
    int32_t maxdepth = 2 * log2(size);
    IntroSort(arr, maxdepth, 0, size);
}

template<typename T>
void IntroSort(T* arr, int32_t maxdepth, int32_t start, int32_t end) {
    int32_t size = end - start;
    if (maxdepth < 100) {
        QuickSort(arr, start, end - 1);
    }
    else if (!maxdepth)
        HeapSort(arr, size);
    else {
        double pivot = (end - start) / 2;
        IntroSort(arr, maxdepth - 1, start, pivot);
        IntroSort(arr, maxdepth - 1, pivot + 1, end);
    }
}

int main() {
    double a[100];
    for (size_t i = 0; i < 100; i++)
    {
        a[i] = rand() % 100;
    }
    IntroSort(a, 100);
    for (size_t i = 0; i < 100; i++)
    {
        std::cout << a[i] << " ";
    }
}