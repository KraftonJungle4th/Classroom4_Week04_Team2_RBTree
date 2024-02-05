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

  node_t *new_node = (node_t*)calloc(1,sizeof(node_t));

  //새 노드 생성
  new_node->key = key;
  new_node->left = new_node->right = t->nil;
  new_node->color = RBTREE_RED;

  // 새 노드를 삽입할 위치 탐색
  node_t *current = t->root;
  node_t *parent = t->nil;

  while(current != t->nil)
  { 
    parent = current;
    if(key < current->key)
    {
      current = current->left;
    } 
    else
      current = current->right;
  }

  //새 노드의 부모 지정
  new_node->parent = parent;
  
  //root가 nil이면 새 노드를 트리의 루트로 지정
  if (parent == t->nil)
  {
      t->root = new_node;
  }
  else if(key < parent->key) 
  {
      parent->left = new_node;
  } 
  else 
  {
      parent->right = new_node;
  }
  
  rbtree_insert_fixup(t, new_node);

  return t->root;
}

//노드 삽입 후 불균형을 복구하는 함수
void rbtree_insert_fixup(rbtree *t, node_t * node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;

  while(parent->color == RBTREE_RED)
  {
    if(parent == grand_parent->left)
    {
      //case1
      if(grand_parent->right->color == RBTREE_RED)
      {
        parent->color = RBTREE_BLACK;
        grand_parent->right->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        node = grand_parent;
      }
      //case2
      else if(node == parent->right)
      {
        node = parent;
        left_rotate(t, node);
        //case3
        parent->color =RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
     
    }
    else
    {
      //case1
      if(grand_parent->left->color == RBTREE_RED)
      {
        parent->color = RBTREE_BLACK;
        grand_parent->left->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        node = grand_parent;
      }
      //case2
      else if(node == parent->right)
      {
        node = parent;
        left_rotate(t, node);
        //case3 
        parent->color =RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
    }     
  }
  t->root->color = RBTREE_BLACK;
}

//왼쪽으로 회전
void left_rotate(rbtree *t, node_t *node)
{
  node_t *y = node->right;
  node->right = y->left;
  if (y->left != t->nil)
    y->left->parent = node;
  
  y->parent = node->parent;
  if(node->parent == t->nil)
    t->root =y;
  else if(node == node->parent->left)
    node->parent->left = y;
  else  
    node->parent->right = y;
  y->left = node;
  node->parent = y;
}

//얼
void right_rotate(rbtree *t, node_t *node)
{
  node_t *y = node->left;
  node->left = y->right;
  if (y->right != t->nil)
    y->right->parent = node;
  
  y->parent = node->parent;
  if(node->parent == t->nil)
    t->root =y;
  else if(node == node->parent->right)
    node->parent->right = y;
  else  
    node->parent->left = y;
  y->right = node;
  node->parent = y;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->left != t->nil)
    current = current->left;
  
  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;
  while(current->right != t->nil)
  current = current->right;
  
  return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
