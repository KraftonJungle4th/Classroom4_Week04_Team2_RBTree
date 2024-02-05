#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));
    if (tree == NULL) {
        return NULL;
    }

    node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
    if (nil_node == NULL) {
        free(tree);
        return NULL;
    }

    nil_node->color = RBTREE_BLACK;
    nil_node->parent = nil_node->left = nil_node->right = nil_node;
    nil_node->key = 0;

    tree->nil = nil_node;

#ifdef SENTINEL
    tree->root = tree->nil;
#else
    tree->root = NULL;
#endif

    return tree;
}


void post_order_free(rbtree *tree, node_t *node);

void delete_rbtree(rbtree *tree) {
    if (tree == NULL) return;

    post_order_free(tree, tree->root);
    free(tree->nil);
    free(tree);
}

void post_order_free(rbtree *tree, node_t *node) {
    if (node == NULL || node == tree->nil) return;

    post_order_free(tree, node->left);
    post_order_free(tree, node->right);
    free(node);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
