#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

void post_order_free(rbtree *tree, node_t *node);
void rbtree_insert_fixup(rbtree *t, node_t *node);
void left_rotate(rbtree *t, node_t *node);
void right_rotate(rbtree *t, node_t *node);
void rbtree_transplant(rbtree *t, node_t *u, node_t *v);
void rbtree_erase_fixup(rbtree *t, node_t *x);

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

  tree->root = nil_node;
  tree->nil = nil_node;

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
  // TODO: implement insert
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  // 새 노드를 삽입할 위치 탐색
  new_node->key = key;
  new_node->left = new_node->right = t->nil;
  new_node->color = RBTREE_RED;

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
  new_node->parent = parent;

  if (parent == t->nil)
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

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  node_t *brother; // 맞나??? 삭제노드x의 형제노드를 가리키는 포인터brother_node를 선언.

  while (x != t->root && x->color == RBTREE_BLACK)
  // 삭제노드가 Root노드가 아니며, 색이 Black이어서 지우기 곤란할 때
  {
    if (x == x->parent->left) // 삭제노드가 왼자식일 때
    {
      brother = x->parent->right; // 형제노드를 삭제노드의 형제로

      // <경우 1> : 경우 2, 3, 4로 위임 가능
      if (brother->color == RBTREE_RED) // 형제노드가 Red일 때 (참고#1)
      {
        brother->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        // 형제노드를 Black으로 바꾸고, 부모노드를 Red로 바꿈( 형제노드가 Red이므로 부모가 무조건 Black이어야 한다 참고#1 )
        left_rotate(t, x->parent); // 삭제노드의 부모를 기준으로 회전
        brother = x->parent->right;
        // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고
        // 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }

      // else (brother->color == RBTREE_BLACK){...} else는 indent 증가로 인해 따로 쓰지않음
      // <경우 2> : x노드가 위쪽으로 바뀌는 경우
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK)
      // 형제의 자식들이 모두 Black이면 (+else까지 합치면 형제와그자식들 모두 Black)
      {
        brother->color = RBTREE_RED; // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;               // 새로운 x
      }
      else // 형제의 자식들이 모두 Black은 아니면 (Red 1개이상 보유) (참고#2)
      {
        // <경우 3> : 경우 4로 위임 가능
        if (brother->right->color == RBTREE_BLACK)
        // 형제의 왼자식 (먼조카) 색이 Black이면 (= 형제의 오른자식(가까조카) 색이 Red 참고#2)
        // 맞나??? 이거 brother->left->color == RBTREE_RED로 바뀌어야 할거같은데
        {
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED; // 형제(black)과 형제자식(red)를 서로 바꾼 후
          right_rotate(t, brother);    // 오른쪽으로 돌린다
          brother = x->parent->right;  // 형제노드를 새로 지정
        }

        // <경우 4> : 특성이 위반되는 것을 해결
        brother->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;      // 형제와 부모색 교환 (형제가 Black인 경우이므로 그냥 Black 하드코딩)
        brother->right->color = RBTREE_BLACK; // 자식 색 검정화
        left_rotate(t, x->parent);
        x = t->root; // 경우 4를 거치면 특성 위반이 해결. x를 root로 설정하여 while문 탈출
      }
    }
    else // 삭제노드가 오른자식일 때 (MIRROR CASE)
    {
      brother = x->parent->left; // 형제노드를 삭제노드의 형제로

      // <경우 1> : 경우 2,3,4로 위임가능
      if (brother->color == RBTREE_RED) // 형제노드가 Red일 때 (참고#1)
      {
        brother->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED; // 형제노드를 Black으로 바꾸고, 부모노드를 Red로 바꿈( 형제노드가 Red이므로 부모가 무조건 Black이어야 한다 참고#1 )
        right_rotate(t, x->parent);    // 삭제노드의 부모를 기준으로 회전
        brother = x->parent->left;
        // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고
        // 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }

      // else (brother->color == RBTREE_BLACK){...} else는 indent 증가로 인해 따로 쓰지않음
      // <경우 2>
      if (brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK)
      // 형제의 자식들이 모두 Black이면 (+else까지 합치면 형제와그자식들 모두 Black)
      {
        brother->color = RBTREE_RED; // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;               // 새로운 x
      }
      else // 형제의 자식들이 모두 Black은 아니면 (Red 1개이상 보유) (참고#2)
      {
        // <경우 3> : 경우 4로 위임 가능
        if (brother->left->color == RBTREE_BLACK)
        // 형제의 왼자식 (먼조카) 색이 Black이면 (= 형제의 오른자식(가까조카) 색이 Red 참고#2)
        // 맞나??? 이거 brother->left->color == RBTREE_RED로 바뀌어야 할거같은데
        {
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED; // 형제(black)과 형제자식(red)를 서로 바꾼 후
          left_rotate(t, brother);     // 왼쪽으로 돌린다
          brother = x->parent->left;   // 형제노드를 새로 지정
        }

        // <경우 4> : 특성이 위반되는 것을 해결
        brother->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;     // 형제와 부모색 교환 (형제가 Black인 경우이므로 그냥 Black 하드코딩)
        brother->left->color = RBTREE_BLACK; // 자식 색 검정화
        right_rotate(t, x->parent);
        x = t->root; // 경우 4를 거치면 특성 위반이 해결. x를 root로 설정하여 while문 탈출
      }
    }
  }

  x->color = RBTREE_BLACK; // x가 root가 되면 Black속성 부여. 양쪽의 Black-height에 영향이 없으므로
}

void inorder(const rbtree *t, node_t *x, key_t *arr, int *idx, const size_t n)
{
  if (x == t->nil)
  {
    return;
  }

  inorder(t, x->left, arr, idx, n);

  if (*idx < n)
  {
    arr[(*idx)++] = x->key;
  }
  else
  {
    return;
  }
  inorder(t, x->right, arr, idx, n);
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  printf("test1");
  node_t *x = t->root;
  if (x == t->nil)
    return 0;
  int cnt = 0;
  int *idx = &cnt;
  printf("test2");
  inorder(t, x, arr, idx, n);
  printf("test3");

  return 0;
}
