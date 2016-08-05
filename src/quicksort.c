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

static const int doshuffle = 0;

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
            arr[0] = temp;
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
void quicksort(int* arr, size_t len)
{
    if (len < 2) {
        return;
    }
    size_t mid = partition(arr, len);
    quicksort(arr, mid);
    quicksort(arr + mid + 1, len - mid - 1);
}

static const size_t NUM_EL = 1000000;
static int ARR[NUM_EL];

int main(int argc, const char* argv[])
{
    // num array elements
    int n = atoi(argv[1]);
    for (size_t i = 0; i < n; ++i) {
        ARR[i] = i;
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
}
