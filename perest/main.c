#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>

#include <perest.h>
#include <btree.h>

void* reallocf(void*, size_t);
perest** build_perest_group(perest**, int*, int);
void print_center(perest**, int);

int fact(int);

int main()
{
    FILE *in, *out;

    if ((in = freopen("input.txt", "r", stdin)) == NULL
        || (out = freopen("output.txt", "w", stdout)) == NULL)
        exit(1);

    int perest_len, group_size;

    scanf("%d%d", &perest_len, &group_size);
    getchar();

    perest** group = (perest**)calloc(group_size, sizeof(perest*));
    for (int i = 0; i < group_size; ++i)
        group[i] = get_perest(NULL, perest_len);

    printf("compositions:\n");
    group = build_perest_group(group, &group_size, fact(group[0]->len));

    printf("group:\n");
    for (int i = 0; i < group_size; ++i) {
        putchar('\t');
        print_perest(group[i], 1);
    }
    printf("center:\n");
    print_center(group, group_size);

    free(group);
    fclose(in);
    fclose(out);
    return 0;
}

void print_center(perest** group, int len)
{
    for (int i = 0; i < len; ++i) {
        int res = 1;
        for (int j = 0; j < len; ++j) {
            perest* a = compose(group[i], group[j]);
            perest* b = compose(group[j], group[i]);
            if (compare(a, b)) {
                res = 0;
                break;
            }
        }
        if (res) {
            putchar('\t');
            print_perest(group[i], 1);
        }
    }
}

#define compose_and_add(a, b)                                                           \
    {                                                                                   \
        flag = 0;                                                                       \
        perest* res = compose(a, b);                                                    \
        root = add_node(root, (void*)res, &flag, (int (*)(void*, void*))compare);       \
        putchar('\t');                                                                  \
        if (flag) {                                                                     \
            group[*len + added++] = res;                                                \
            printf("added : ");                                                         \
        }                                                                               \
        print_perest(a, 0);                                                             \
        print_perest(b, 0);                                                             \
        printf("= ");                                                                   \
        print_perest(res, 1);                                                           \
        if (flag)                                                                       \
            putchar('\n');                                                              \
    }

perest** build_perest_group(perest** group, int* len, int max_len)
{
    btree* root = NULL;
    int old_len;
    for (int i = 0; i < *len; ++i)
        root = add_node(root, (void*)group[i], NULL, (int (*)(void*, void*))compare);
    old_len = 0;
    group = (perest**)reallocf((void*)group, max_len * sizeof(perest*));
    do {
        int added = 0;
        for (int i = *len - 1; i >= old_len; --i) {
            if (group[i]->cycle != NULL && !strcmp(group[i]->cycle, "e"))
                continue;
            int flag;
            compose_and_add(group[i], group[i]);
            for (int j = i - 1; j >= 0; --j) {
                if (group[j]->cycle != NULL && !strcmp(group[j]->cycle, "e"))
                    continue;
                compose_and_add(group[i], group[j]);
                compose_and_add(group[j], group[i]);
            }
        }
        old_len = *len;
        *len += added;
    } while (*len != old_len);
    group = (perest**)reallocf((void*)group, (*len) * sizeof(perest*));
    return group;
}

void* reallocf(void* ptr, size_t new_size)
{
    void* new_ptr = realloc(ptr, new_size);
    if (new_ptr == NULL) {
        free(ptr);
        fprintf(stderr, "FATAL: unable to reallocate memory.\n");
        exit(1);
    }
    return new_ptr;
}

int fact(int x)
{
    int retval = 1;
    for (; x > 1; --x)
        retval *= x;
    return retval;
}