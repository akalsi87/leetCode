// quicksort.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
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
    int ct = 1;
    for (size_t i = 0; i < len; ++i) {
        if (ct % 20 != 0) {
            printf("%d, ", arr[i]);
        } else {
            printf("%d\n", arr[i]);
        }
        ++ct;
    }
    puts("\b]");
}

static const int doshuffle = 0;
static const int domedian = 1;

static
int issorted(int* arr, size_t len)
{
    if (len <= 1) {
        return 1;
    }
    for (size_t i = 0; i < len - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            fprintf(stderr, "Found %d before %d at index %zu\n", arr[i], arr[i + 1], i);
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
void partition(int* arr, size_t len, size_t* plt, size_t* pgt)
{
    if (len < 2) {
        *plt = *pgt = 0;
        return;
    }

    if (len == 2) {
        if (arr[0] > arr[1]) {
            int temp = arr[0];
            arr[0] = arr[1];
            arr[1] = temp;
        }
        *plt = 0;
        *pgt = 1;
        return;
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

    size_t lt = 0;
    size_t i = 0;
    size_t j = len - 1;

    while (i <= j) {
        if (arr[i] < pivot) {
            int temp = arr[lt];
            arr[lt] = arr[i];
            arr[i] = temp;
            ++lt;
            ++i;
        } else if (arr[i] == pivot) {
            ++i;
        } else {
            int temp = arr[j];
            arr[j] = arr[i];
            arr[i] = temp;
            --j;
        }
    }
    
    *plt = lt;
    *pgt = i;
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

static const size_t CUTOFF = 800;

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
    
    size_t lt;
    size_t gt;
    partition(arr, len, &lt, &gt);
    quicksort(arr, lt);
    assert(issorted(arr, lt));
    quicksort(arr + gt, len - gt);
    assert(issorted(arr + gt, len - gt));
}

static int* ARR = 0;

int main(int argc, const char* argv[])
{
    // num array elements
    int n = atoi(argv[1]);
    ARR = malloc(sizeof(int)*n);
    
    FILE* f = fopen(argv[2], "r");
    char buf[80] = { '\0' };
    size_t sz = 79;
    for (size_t i = 0; i < n; ++i) {
        ARR[i] = atoi(fgets(buf, sz, f));
    }
    fclose(f);

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
