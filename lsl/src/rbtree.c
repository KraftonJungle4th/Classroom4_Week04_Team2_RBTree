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
int rbtree_erase(rbtree *t, node_t *erase)
{
  node_t *y = erase;
  // 삭제할노드를 가리키는 포인터 y (후에 successor가 됨)
  color_t y_try_to_erase_color = y->color;
  // 삭제할 컬러를 미리 저장
  node_t *e_child = NULL;
  // 맞나??? y의 자식포인터 x 선언

  if (erase->left == t->nil)
  // 참이면 오른자식만 있거나 둘다없음
  {
    e_child = erase->right;
    // 노드의 오른자식을 가리키게 함
    rbtree_transplant(t, erase, erase->right);
  }
  else if (erase->right == t->nil)
  // 참이면 왼자식만 있음
  {
    e_child = erase->left;
    // 노드의 왼자식을 가리키게 함
    rbtree_transplant(t, erase, erase->left);
  }
  else
  // 노드의 자식이 2명
  {
    y = rbtree_min(erase->right);
    // 삭제될노드의 계승자를 찾는다
    y_try_to_erase_color = y->color;
    // 삭제할 노드의 색을 저장한다
    e_child = y->right;
    // 대체할노드에 삭제할 노드의 오른자식 할당 (succesor 찾음)
    if (y->parent == erase)
    // successor의 부모가 바로 지워야 할 위치일때
    {
      e_child->parent = y;
      // erase자리를 erase자식의 부모로 바로할당
    }
    else
    // successor가 몇칸 내려가는 경우일 때,
    {
      rbtree_transplant(t, y, y->right);
      // y자리의 오른쪽 부분 기준 나무심음
      y->right = erase->right;
      // 지울 노드의 오른자식을 y위치의 오른자식으로
      y->right->parent = y;
    }

    rbtree_transplant(y, erase, y->right);
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

void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  node_t *brother = NULL;
  // 맞나??? 삭제노드x의 형제노드를 가리키는 포인터brother_node를 선언.

  while (x != t->root && x->color == RBTREE_BLACK)
  // 삭제노드가 Root노드가 아니며, 색이 Black이어서 지우기 곤란할 때
  {
    if (x == x->parent->left)
    // 삭제노드가 왼자식일 때
    {
      brother = x->parent->right;
      // 형제노드를 삭제노드의 형제로

      // <경우 1> : 경우 2, 3, 4로 위임 가능
      if (brother->color == RBTREE_RED)
      // 형제노드가 Red일 때 (참고#1)
      {
        brother->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        // 형제노드를 Black으로 바꾸고, 부모노드를 Red로 바꿈( 형제노드가 Red이므로 부모가 무조건 Black이어야 한다 참고#1 )
        left_rotate(t, x->parent);
        // 삭제노드의 부모를 기준으로 회전
        brother = x->parent->right;
        // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고
        // 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }

      // else (brother->color == RBTREE_BLACK){...} else는 indent 증가로 인해 따로 쓰지않음
      // <경우 2> : x노드가 위쪽으로 바뀌는 경우
      if (brother->left->color == RBTREE_BLACK && brother->right->color == RBTREE_BLACK)
      // 형제의 자식들이 모두 Black이면 (+else까지 합치면 형제와그자식들 모두 Black)
      {
        brother->color = RBTREE_RED;
        // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;
        // 새로운 x
      }
      else
      // 형제의 자식들이 모두 Black은 아니면 (Red 1개이상 보유) (참고#2)
      {
        // <경우 3> : 경우 4로 위임 가능
        if (brother->right->color == RBTREE_BLACK)
        // 형제의 왼자식 (먼조카) 색이 Black이면 (= 형제의 오른자식(가까조카) 색이 Red 참고#2)
        // 맞나??? 이거 brother->left->color == RBTREE_RED로 바뀌어야 할거같은데
        {
          brother->left->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          // 형제(black)과 형제자식(red)를 서로 바꾼 후
          right_rotate(t, brother);
          // 오른쪽으로 돌린다
          brother = x->parent->right;
          // 형제노드를 새로 지정
        }

        // <경우 4> : 특성이 위반되는 것을 해결
        brother->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        // 형제와 부모색 교환 (형제가 Black인 경우이므로 그냥 Black 하드코딩)
        brother->right->color = RBTREE_BLACK;
        // 자식 색 검정화
        left_rotate(t, x->parent);
        x = t->root;
        // 경우 4를 거치면 특성 위반이 해결. x를 root로 설정하여 while문 탈출
      }
    }
    else
    // 삭제노드가 오른자식일 때 (MIRROR CASE)
    {
      brother = x->parent->left;
      // 형제노드를 삭제노드의 형제로

      // <경우 1> : 경우 2,3,4로 위임가능
      if (brother->color == RBTREE_RED)
      // 형제노드가 Red일 때 (참고#1)
      {
        brother->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        // 형제노드를 Black으로 바꾸고, 부모노드를 Red로 바꿈( 형제노드가 Red이므로 부모가 무조건 Black이어야 한다 참고#1 )
        right_rotate(t, x->parent);
        // 삭제노드의 부모를 기준으로 회전
        brother = x->parent->left;
        // 회전을 끝내고 난 후에는 x->parent->right가 새로운 노드가 되고
        // 밑의 if, else if, else 중 한 가지, 즉 경우 2, 3, 4의 한 가지로 위임된다.
      }

      // else (brother->color == RBTREE_BLACK){...} else는 indent 증가로 인해 따로 쓰지않음
      // <경우 2>
      if (brother->right->color == RBTREE_BLACK && brother->left->color == RBTREE_BLACK)
      {
        // 형제의 자식들이 모두 Black이면 (+else까지 합치면 형제와그자식들 모두 Black)
        brother->color = RBTREE_RED;
        // x의 extra black을 x->parent로 넘긴다. 그러면서 w는 red가 된다.
        x = x->parent;
        // 새로운 x
      }
      else
      // 형제의 자식들이 모두 Black은 아니면 (Red 1개이상 보유) (참고#2)
      {
        // <경우 3> : 경우 4로 위임 가능
        if (brother->left->color == RBTREE_BLACK)
        // 형제의 왼자식 (먼조카) 색이 Black이면 (= 형제의 오른자식(가까조카) 색이 Red 참고#2)
        // 맞나??? 이거 brother->left->color == RBTREE_RED로 바뀌어야 할거같은데
        {
          brother->right->color = RBTREE_BLACK;
          brother->color = RBTREE_RED;
          // 형제(black)과 형제자식(red)를 서로 바꾼 후
          left_rotate(t, brother);
          // 왼쪽으로 돌린다
          brother = x->parent->left;
          // 형제노드를 새로 지정
        }

        // <경우 4> : 특성이 위반되는 것을 해결
        brother->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        // 형제와 부모색 교환 (형제가 Black인 경우이므로 그냥 Black 하드코딩)
        brother->left->color = RBTREE_BLACK;
        // 자식 색 검정화
        right_rotate(t, x->parent);
        x = t->root;
        // 경우 4를 거치면 특성 위반이 해결. x를 root로 설정하여 while문 탈출
      }
    }
  }

  x->color = RBTREE_BLACK;
  // x가 root가 되면 Black속성 부여. 양쪽의 Black-height에 영향이 없으므로
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
  // TODO: implement to_array
  return 0;
}
