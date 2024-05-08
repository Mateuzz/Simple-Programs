#pragma once

#include <stddef.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

void merge_sort(int *array, size_t high);
//quick_sort(array, &array[tamanho])
void quick_sort(int *low, int *high);
// insertion_sort(array, tamanho)
void insertion_sort(int *array, size_t size);

int *split(int *low, int *high);


void insertion_sort(int *array, const size_t size)
{
    int i, j;
    int tmp;

    for (i = 0; i < size - 1; i++) {
        if (array[i + 1] < array[i]) {
            tmp = array[i + 1];
            j = i;
            while (tmp < array[j] && j >= 0) {
                array[j + 1] = array[j];
                j--;
            }
            array[j + 1] = tmp;
        }
    }
}

void Merge2(int *restrict array, size_t begin, size_t middle, size_t end) {
    size_t size1 = middle - begin + 1;
    size_t size2 = end - middle;
    int left[size1 + 1];
    int right[size2 + 1];

    memcpy(left, array + begin, sizeof(left) - sizeof(int));
    memcpy(right, array + middle + 1, sizeof(right) - sizeof(int));

    left[size1] = right[size2] = INT_MAX;

    for (size_t i = 0, j = 0; begin <= end; ++begin) {
        if (left[i] < right[j]) {
            array[begin] = left[i++];
        } else {
            array[begin] = right[j++];
        }
    }
}

void merge_sort(int *array, const size_t high)
{
    if (high <= 1) return;
    
    merge_sort(array, high / 2);
    if (high % 2 == 0)
        merge_sort(&array[high / 2], high / 2);
    else 
        merge_sort(&array[high / 2], high / 2 + 1);

    int tmp[high];
    for (int i = 0, k = 0, j = high / 2; k < high;) {
        if ((j >= high) || (array[i] < array[j] && i < high / 2))
            tmp[k++] = array[i++];
        else
            tmp[k++] = array[j++];
    }

    for (int k = 0; k < high; k++)
        array[k] = tmp[k];
}

void quick_sort(int *low, int *high)
{
    if (low >= high)
        return;
    int *middle = split(low, high);
    quick_sort(low, middle - 1);
    quick_sort(middle + 1, high);
}

int *split(int *low, int *high)
{
    int pivot =  *low;
    for (;;) {
        while (low < high && *high >= pivot)
            high--;
        if (low >= high)
            break;
        *low++ = *high;
        
        while (low < high && *low <= pivot)
            low++;
        if (low >= high)
            break;
        *high-- = *low;
    }
    *high = pivot;
    return high;
}

void qsort4(int *x, int l, int u) {
    int n = rand() % (u - l + 1) + l;
    int tmp = x[n];
    x[n] = x[l];
    x[l] = tmp;
    int t = x[l];
    int i = l;
    int j = u + 1;
    for (;;) {
        while (i <= u && x[i] < t)
            ++i;
        while (x[j] > t)
            --j;
        if (i > j)
            break;
        tmp = x[i];
        x[i] = x[j];
        x[j] = tmp;
    }

    tmp = x[l];
    x[l] = x[j];
    x[j] = tmp;

    qsort4(x, l, j-1);
    qsort4(x, j+l, u);
}

