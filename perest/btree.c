#include "btree.h"

#include <stdlib.h>

btree* add_node(btree* p, void* item, int* is_added, int (*cmp)(void*, void*)) {
	if (p == NULL) {
		p =(btree*)malloc(sizeof(btree));
		p->left = p->right = NULL;
		p->key = item;
		if (is_added != NULL)
			*is_added = 1;
	}
	else {
		int cond = cmp(p->key, item);
		if (cond > 0)
			p->left = add_node(p->left, item, is_added, cmp);
		else if (cond < 0)
			p->right = add_node(p->right, item, is_added, cmp);
	}
	return p;
}

