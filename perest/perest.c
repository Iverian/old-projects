#include "perest.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_naive(perest* x, char flag)
{
	printf("\t[ ");
	for (int i = 0; i < x->len; ++i)
		printf("%d ", x->val[i]);
	printf("]%c", (flag) ? '\n' : ' ');
}

int compare(perest* a, perest* b)
{
	int retval = 0;
	if (a->len == b->len) {
		for (int i = 0; i < a->len; ++i)
			if (a->val[i] != b->val[i]) {
				retval = a->val[i] - b->val[i];
				break;
			}
	}
	else
		retval = a->len - b->len;
	return retval;
}

perest* compose(perest* a, perest* b)
{
	perest* retval = copy(b);
	char* flags = (char*)calloc(b->len, sizeof(char));
	for (int i = 0; i < a->len; ++i) {
		for (int j = 0; j < retval->len; ++j)
			if (retval->val[j] == i && flags[j] == 0) {
				retval->val[j] = a->val[i];
				flags[j] = 1;
			}
	}
	free(flags);
	return retval;
}

perest* copy(perest* src)
{
	perest* retval = new_perest(src->len);
	for (int i = 0; i < src->len; ++i)
		retval->val[i] = src->val[i];
	return retval;
}

#define BUFSIZE 101

void print_perest(perest* x, char flag)
{
	if (x->cycle == NULL)
		x->cycle = to_cycle(x);
	printf("%s%c", x->cycle, (flag) ? '\n' : ' ');
}

char* to_cycle(perest* x)
{
	if (x->cycle != NULL)
		return x->cycle;
	int pos = 0, status;
	x->cycle = (char*)calloc(BUFSIZE, sizeof(char));
	char* is_printed = (char*)calloc(x->len, sizeof(char));
	for (int i = status = 0; i < x->len; ++i) {
		if (is_printed[i] == 1 || x->val[i] == i)
			continue;
		x->cycle[pos++] = '(';
		int j = i;
		do {
			pos += sprintf(x->cycle + pos, "%d", j + 1);
			++status;
			is_printed[j] = 1;
		} while (j = x->val[j], (j != i) ? (x->cycle[pos++] = ' ') : 0, j != i);
		x->cycle[pos++] = ')';
	}
	if (!status)
		x->cycle[pos++] = 'e';
	free(is_printed);
	x->cycle = (char*)realloc((void*)x->cycle, pos + 1);
	return x->cycle;
}

#define extract(dst, src, tok, size, len)                                                          \
	{                                                                                              \
		dst = (char**)calloc(size + 1, sizeof(char*));                                             \
		char* x = strtok(src, tok);                                                                \
		for (len = 0; x != NULL; x = strtok(NULL, tok)) {                                          \
			dst[len++] = x;                                                                        \
		}                                                                                          \
	}

int find_val(int, int*, uint);
void swap(int*, int*);

perest* get_perest(char* str, uint len)
{
	perest* retval = new_perest(len);
	char* buf;
	if (str == NULL) {
		int i = 0;
		buf = (char*)calloc(BUFSIZE, sizeof(char));
		fgets(buf, BUFSIZE, stdin);
		if (buf[strlen(buf) - 1] == '\n')
			i = 1;
		retval->cycle = (char*)calloc(strlen(buf) + 1 - i, sizeof(char));
		strncpy(retval->cycle, buf, strlen(buf) - i);
	}
	else {
		buf = (char*)calloc(strlen(str) + 1, sizeof(char));
		strncpy(buf, str, strlen(str));
		retval->cycle = str;
	}
	if (!strchr(buf, 'e')) {
		int tlen;
		char** toks;
		extract(toks, buf, "()\n", len, tlen);
		while (tlen-- > 0) {
			char* ccl[2] = { strtok(toks[tlen], " _"), strtok(NULL, " _") };
			int no[2] = { atoi(ccl[0]) - 1, atoi(ccl[1]) - 1 };
			while (ccl[1] != NULL) {
				swap(&retval->val[find_val(no[0], retval->val, retval->len)],
					&retval->val[find_val(no[1], retval->val, retval->len)]);
				ccl[0] = ccl[1];
				no[0] = no[1];
				ccl[1] = strtok(NULL, " _");
				no[1] = atoi(ccl[1]) - 1;
			}
		}
		free(toks);
	}
	free(buf);
	return retval;
}

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int find_val(int val, int* arr, uint len)
{
	int retval = -1;
	for (uint i = 0; i < len; ++i)
		if (val == arr[i]) {
			retval = i;
			break;
		}
	return retval;
}

perest* new_perest(uint len)
{
	perest* retval = (perest*)malloc(sizeof(perest));
	retval->cycle = NULL;
	retval->len = len;
	retval->val = (int*)calloc(len, sizeof(int));
	for (int i = 0; i < len; ++i)
		retval->val[i] = i;
	return retval;
}
