// rbtree.c

#include "common.h"

typedef struct rb_int_node_tag rb_int_node;

struct rb_int_node_tag
{
    rb_int_node* parent;
    rb_int_node* left;
    rb_int_node* right;
    int value;
};

rb_int_node* rb_int_tree_create_node(int v)
{
    return 0;
}

rb_int_node* rb_int_tree_insert(rb_int_node* root, int v)
{
    return 0;
}

rb_int_node* rb_int_tree_create(int v)
{
    return 0;
}

static int* ARR = 0;
rb_int_node* ROOT = 0;

void createrbtree(int* arr, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        if (ROOT) {
            rb_int_tree_insert(ROOT, arr[i]);
        } else {
            ROOT = rb_int_tree_create(arr[i]);
        }
    }
}

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

    puts("Sorting...");
    markstart();
    createrbtree(ARR, n);
    markend();
    puts("Sorted...");

    return 0;
}

