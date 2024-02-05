#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {

  //tree 구주체 동적 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  //nil 노드 생성 및 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;

  //tree의 nil과 root를 nil노드로 설정(tree가 빈 경우 root는 nil노드여야 함)
  p->nil = p->root = nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t * new_node =(node_t*)calloc(1,sizeof(node_t*));
  node_t *y = t->nil;
  node_t *x = t->root;
  new_node->key = key;

  while(x != y)
  {
    y = x;
    if (key < x->key)
      x = x->left;
  }

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
