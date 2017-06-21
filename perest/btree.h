typedef struct bt {
	struct bt* left;
	struct bt* right;
	void* key;
} btree;

btree* add_node(btree*, void* , int*, int (*)(void*, void*));