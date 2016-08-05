// quicksort.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/time.h>

struct timeval start;
struct timeval end;

static
void markstart()
{
    gettimeofday(&start, NULL);
}

static
void markend()
{
    gettimeofday(&end, NULL);
    long secs = (end.tv_sec - start.tv_sec); //avoid overflow by subtracting first
    long micros = ((secs*1000000) + end.tv_usec) - (start.tv_usec);
    printf("microseconds taken: %d\n", (int)micros);
}

static
uint64_t randnum()
{
    /* These state variables must be initialized so that they are not all zero. */
    static uint64_t s[2] = { 1, (uint64_t)-1 };
    uint64_t x = s[0];
    uint64_t const y = s[1];
    s[0] = y;
    x ^= x << 23; // a
    s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
    return s[1] + y;
}

static
void printarray(int* arr, size_t len)
{
    puts("[");
    for (size_t i = 0; i < len; ++i) {
        printf("%d\n", arr[i]);
    }
    puts("]");
}

static const int doshuffle = 0;
static const int domedian = 0;

static
int issorted(int* arr, size_t len)
{
    for (size_t i = 0; i < len - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

static
void shuffle(int* arr, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        size_t j = randnum() % (i + 1);
        int temp = arr[j];
        arr[j] = arr[i];
        arr[i] = temp;
    }
}

static
size_t partition(int* arr, size_t len)
{
    if (len < 2) {
        return 0;
    }
    
    int pivotidx;
    if (doshuffle) {
        shuffle(arr, len);
        pivotidx = 0;
    } else if (domedian) {
        size_t x = randnum() % len;
        size_t y = randnum() % len;
        size_t z = randnum() % len;
        if (arr[x] < arr[y]) {
            if (arr[y] < arr[z]) {
                pivotidx = y;
            } else if (arr[x] < arr[z]) {
                pivotidx = z;
            } else {
                pivotidx = x;
            }
        } else {
            if (arr[x] < arr[z]) {
                pivotidx = x;
            } else if (arr[z] < arr[y]) {
                pivotidx = y;
            } else {
                pivotidx = z;
            }
        }
    } else {
        pivotidx = len/2;
    }

    int pivot = arr[pivotidx];

    size_t i = 0;
    size_t j = len - 1;
    while (1) {
        while (i < len) {
            if (arr[i] < pivot) {
                ++i;
            } else {
                break;
            }
            if (i >= j) {
                break;
            }
        }

        while (j >= i) {
            if (arr[j] > pivot) {
                --j;
            } else  {
                break;
            }
        }

        if (i >= j) {
            int temp = arr[j];
            arr[j] = pivot;
            arr[pivotidx] = temp;
            break;
        } else {
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
            /* continue the loop */
        }
    }

    return j;
}

static
void insertionsort(int* arr, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        int val = arr[i];
        size_t j = i;
        for (; j >= 1; --j) {
            if (arr[j - 1] > val) {
                arr[j] = arr[j - 1];
            } else {
                break;
            }
        }
        arr[j] = val;
    }
}

static const size_t CUTOFF = 1000;

static
void quicksort(int* arr, size_t len)
{
    if (len < 2) {
        return;
    }

    if (len < CUTOFF) {
        insertionsort(arr, len);
        return;
    }

    size_t mid = partition(arr, len);
    quicksort(arr, mid);
    quicksort(arr + mid + 1, len - mid - 1);
}

static int* ARR = 0;

int main(int argc, const char* argv[])
{
    // num array elements
    int n = atoi(argv[1]);
    ARR = malloc(sizeof(int)*n);
    for (size_t i = 0; i < n; ++i) {
        ARR[i] = (n-i);
    }

    //shuffle(ARR, n);
    (void)printarray;
    //printarray(ARR, n);

    puts("Sorting...");
    markstart();
    quicksort(ARR, n);
    markend();
    puts("Sorted...");
    //printarray(ARR, n);

    return !issorted(ARR, n);
}
