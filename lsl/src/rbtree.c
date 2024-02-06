#include "rbtree.h"

#include <stdlib.h>

void post_order_free(rbtree *tree, node_t *node);

rbtree *new_rbtree(void)
{
  rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));
  if (tree == NULL)
  {
    return NULL;
  }

  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  if (nil_node == NULL)
  {
    free(tree);
    return NULL;
  }

  nil_node->color = RBTREE_BLACK;
  nil_node->parent = nil_node->left = nil_node->right = nil_node;
  nil_node->key = 0;

  tree->nil = tree->root = nil_node;

  return tree;
}

void delete_rbtree(rbtree *tree)
{
  if (tree == NULL)
    return;

  post_order_free(tree, tree->root);
  free(tree->nil);
  free(tree);
}

void post_order_free(rbtree *tree, node_t *node)
{
  if (node == NULL || node == tree->nil)
    return;

  post_order_free(tree, node->left);
  post_order_free(tree, node->right);
  free(node);
}

// 왼쪽으로 회전
void left_rotate(rbtree *t, node_t *node)
{
  node_t *y = node->right;
  node->right = y->left;
  if (y->left != t->nil)
    y->left->parent = node;

  y->parent = node->parent;
  if (node->parent == t->nil)
    t->root = y;
  else if (node == node->parent->left)
    node->parent->left = y;
  else
    node->parent->right = y;
  y->left = node;
  node->parent = y;
}

// 얼
void right_rotate(rbtree *t, node_t *node)
{
  node_t *y = node->left;
  node->left = y->right;
  if (y->right != t->nil)
    y->right->parent = node;

  y->parent = node->parent;
  if (node->parent == t->nil)
    t->root = y;
  else if (node == node->parent->right)
    node->parent->right = y;
  else
    node->parent->left = y;
  y->right = node;
  node->parent = y;
}

// 노드 삽입 후 불균형을 복구하는 함수
void rbtree_insert_fixup(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;

  while (parent->color == RBTREE_RED)
  {
    if (parent == grand_parent->left)
    {
      // case1
      if (grand_parent->right->color == RBTREE_RED)
      {
        parent->color = RBTREE_BLACK;
        grand_parent->right->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        node = grand_parent;
      }
      // case2
      else if (node == parent->right)
      {
        node = parent;
        left_rotate(t, node);
        // case3
        parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
    }
    else
    {
      // case1
      if (grand_parent->left->color == RBTREE_RED)
      {
        parent->color = RBTREE_BLACK;
        grand_parent->left->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        node = grand_parent;
      }
      // case2
      else if (node == parent->right)
      {
        node = parent;
        left_rotate(t, node);
        // case3
        parent->color = RBTREE_BLACK;
        grand_parent->color = RBTREE_RED;
        right_rotate(t, grand_parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{

  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  new_node->key = key; // 새 노드 생성
  new_node->left = new_node->right = t->nil;
  new_node->color = RBTREE_RED;

  // 새 노드를 삽입할 위치 탐색
  node_t *current = t->root;
  node_t *parent = t->nil;

  while (current != t->nil)
  {
    parent = current;
    if (key < current->key)
    {
      current = current->left;
    }
    else
      current = current->right;
  }

  new_node->parent = parent; // 새 노드의 부모 지정

  if (parent == t->nil) // root가 nil이면 새 노드를 트리의 루트로 지정
  {
    t->root = new_node;
  }
  else if (key < parent->key)
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

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *current = t->root;
  while (current->left != t->nil)
    current = current->left;

  return current;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *current = t->root;
  while (current->right != t->nil)
    current = current->right;

  return current;
}

node_t *rbtree_min_succ(rbtree *t, node_t *y)
{
  node_t *r = y;
  if (r == t->nil) // 새로 추가
    return r;
  while (r->left != t->nil) // y의 왼쪽 자식이 nil이 아닐 때까지 계속 파고들어간다.
  {
    r = r->left;
  }

  // y의 왼쪽 자식이 nil이라면 멈추기 때문에(y가 nil이면 멈추는게 아니라) y는 유의미한 값을 가진 노드를 가르키는 주소값이다. 여기서는 successor라고 보면 된다.
  return r;
}

/* 삭제하려는 노드의 대체노드에 각종 삭제정보를 이식하는 함수 */
void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  // 삭제할 노드 u가 트리의 Root노드면, 대체노드 v를 트리의 Root노드로 삼는다
  else if (u == u->parent->left)
    u->parent->left = v;
  // 삭제할 노드 u자신이 왼자식이면, 대체노드 v를 삭제할 노드 u의 부모의 왼자식으로 삼는다
  else
    u->parent->right = v;
  // 둘다 아니면 = 삭제할 노드 u자식이 오른자식이면, 대체노드 v를 삭제할 노드 u의 부모의 오른자식으로 삼는다

  v->parent = u->parent; // 삭제할 노드 u의 부모를 대체노드 v의 부모로 삼는다 (부모가 nil이어도 상관x)
}

void rbtree_erase_fixup(rbtree *t, node_t *x);

/* 트리t에서 노드e_node를 삭제하는 함수 */
int rbtree_erase(rbtree *t, node_t *erase)
{
  node_t *y = erase;                       // 삭제할노드를 가리키는 포인터 y (후에 successor가 됨)
  color_t y_try_to_erase_color = y->color; // 삭제할 컬러를 미리 저장
  node_t *e_child;                         // 맞나??? y의 자식포인터 x 선언

  if (erase->left == t->nil) // 참이면 오른자식만 있거나 둘다없음
  {
    e_child = erase->right; // 노드의 오른자식을 가리키게 함
    rbtree_transplant(t, erase, erase->right);
  }
  else if (erase->right == t->nil) // 참이면 왼자식만 있음
  {
    e_child = erase->left; // 노드의 왼자식을 가리키게 함
    rbtree_transplant(t, erase, erase->left);
  }
  else // 노드의 자식이 2명
  {
    y = rbtree_min_succ(t, erase->right); // 삭제될노드의 계승자를 찾는다
    y_try_to_erase_color = y->color;      // 삭제할 노드의 색을 저장한다
    e_child = y->right;                   // 대체할노드에 삭제할 노드의 오른자식 할당 (succesor 찾음)
    if (y->parent == erase)               // successor의 부모가 바로 지워야 할 위치일때
    {
      e_child->parent = y; // erase자리를 erase자식의 부모로 바로할당
    }
    else // successor가 몇칸 내려가는 경우일 때,
    {
      rbtree_transplant(t, y, y->right); // y자리의 오른쪽 부분 기준 나무심음
      y->right = erase->right;           // 지울 노드의 오른자식을 y위치의 오른자식으로
      y->right->parent = y;
    }

    rbtree_transplant(t, erase, y->right);
    // 실제 심는과정
    y->left = erase->left;
    y->left->parent = y;
    y->color = erase->color;
  }

  if (y_try_to_erase_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, erase);
  }

  free(erase);
  return 0;
}

/* 삭제하려는 노드의 대체노드에 각종 삭제정보를 이식하는 함수 */
void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  // 삭제할 노드 u가 트리의 Root노드면,
  {
    t->root = v;
    // 대체노드 v를 트리의 Root노드로 삼는다
  }
  else if (u == u->parent->left)
  // 삭제할 노드 u자신이 왼자식이면,
  {
    u->parent->left = v;
    // 대체노드 v를 삭제할 노드 u의 부모의 왼자식으로 삼는다
  }
  else
  // 둘다 아니면 = 삭제할 노드 u자식이 오른자식이면,
  {
    u->parent->right = v;
    // 대체노드 v를 삭제할 노드 u의 부모의 오른자식으로 삼는다
  }

  v->parent = u->parent;
  // 삭제할 노드 u의 부모를 대체노드 v의 부모로 삼는다 (부모가 nil이어도 상관x)
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  if (t->root == t->nil)
  {
    return -1;
  }
  inorder_rbtree(t->root, arr, t, 0);
  return 0;
}
