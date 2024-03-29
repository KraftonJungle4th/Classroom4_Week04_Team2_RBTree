#include "rbtree.h"
#include <stdlib.h>

void post_order_free(rbtree *tree, node_t *node);
void rbtree_insert_fixup(rbtree *t, node_t * node);
void left_rotate(rbtree *t, node_t *node);
void right_rotate(rbtree *t, node_t *node);

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

    tree->root = nil_node;
    tree->nil = nil_node;

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
  node_t *new_node = (node_t*)calloc(1,sizeof(node_t));
  // 새 노드를 삽입할 위치 탐색
  new_node->key = key;
  new_node->left = new_node->right = t->nil;
  new_node->color = RBTREE_RED;

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
  new_node->parent = parent;
      
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
    node_t *current = t->root;

    while (current != t->nil) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return NULL;
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
