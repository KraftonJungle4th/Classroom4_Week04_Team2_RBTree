#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
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
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

/* 트리t에서 노드e_node를 삭제하는 함수 */
int rbtree_erase(rbtree *t, node_t *e_node)
{
  node_t *y = e_node;
  // 삭제할노드를 가리키는 포인터 y
  color_t y_try_to_erase_color = y->color;
  // 삭제할 컬러를 미리 저장
  node_t *x = NULL;
  // 맞나??? 대체포인터 x 선언

  if (e_node->left == t->nil)
  // 참이면 오른자식만 있거나 둘다없음
  {
    x = e_node->right;
    // 노드의 오른자식을 가리키게 함
    rbtree_transplant(t, e_node, e_node->right);
  }
  else if (e_node->right == t->nil)
  // 참이면 왼자식만 있음
  {
    x = e_node->left;
    // 노드의 왼자식을 가리키게 함
    rbtree_transplant(t, e_node, e_node->left);
  }
  else
  // 노드의 자식이 2명
  {
    y = rbtree_min(e_node->right);
    // 삭제될노드의 계승자를 찾는다
    y_try_to_erase_color = y->color;
    // 삭제할 노드의 색을 저장한다
    x = y->right;
    // 대체할노드에 삭제할 노드의 오른자식 할당
    if (y->parent == e_node)
    {
      x->parent = y;
    }
    else
    {
      rbtree_transplant(t, y, y->right);
      y->right = e_node->right;
      y->right->parent = y;
    }

    rbtree_transplant(y, e_node, y->right);
    y->left = e_node->left;
    y->left->parent = y;
    y->color = e_node->color;
  }

  if (y_try_to_erase_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, e_node);
  }

  // TODO: implement erase

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

/* 삭제 후, 재조정하는 함수 */
void rbtree_erase_fixup(rbtree *t, node_t *e_node)
{
  node_t *b_node;
  // 삭제할노드e_node의 형제노드b_node 포인터를 선언

  while (e_node != t->root && e_node->color == RBTREE_BLACK)
  { // x가 root가 되면 단순히 그냥 검은색으로 바꾸면된다. 그리고 while문 아래는 x가 doubly black일 떄 이뤄진다.
    // doubly black인 x가 왼쪽 자식일 때
    if (x == x->parent->left)
    {
      w = x->parent->right;

      // <경우 1> : 경우 2, 3, 4로 위임 가능
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right; // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }

      // 위의 if문을 만나지 않았으므로, w->color == RBTREE_BLACK인 경우이다.
      // <경우 2> : 경우 1, 2, 3, 4로 위임 가능
      // x->parent로 짬 때리는 경우이다.
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED; // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;         // 새롭게 doubly black 혹은 red and black이 x->parent이 짬 맞아서 재조정을 진행하도록 위임한다.
      }
      else
      {

        // <경우 3> : 경우 4로 위임 가능
        if (w->right->color == RBTREE_BLACK)
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        // <경우 4> : 특성이 위반되는 것을 해결한다. 경우 4는 다른 경우로 위임되지 않고 위반을 해결(특성을 만족)한다.
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root; // 경우 4를 거치면 특성 위반이 해결되는 것이므로 x를 root로 설정하여 while문을 빠져나가도록 한다.
      }

      // doubly black인 x가 오른쪽 자식일 때
    }
    else
    {
      w = x->parent->left;

      // <경우 1>
      if (w->color == RBTREE_RED)
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      // <경우 2>
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK)
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {

        // <경우 3>
        if (w->left->color == RBTREE_BLACK)
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        // <경우 4>
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
