#include "rbtree.h"

#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////////

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // nil 노드 생성 및 초기화
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node->color = RBTREE_BLACK;

  // 트리의 nil 및 root 설정
  p->nil = nil_node;
  p->root = p->nil;

  return p;
}

//////////////////////////////////////////////////////////////////////////////////////

/*
1. a node is either red of black
2. the root and leaves(NIL) are black
3. if a node is red, then its children are black
4. all paths from a node to its NIL descendants contain the same nubmer of black nodes

extra note :
1. nodes require one storage bit to keep track of color
2. the longest path (root to farthest nil) is no more than twice the lenght of the shortest path
shortest path -> all black nodes / longest path -> alternating red and black
*/
void delete_nodes(rbtree *t, node_t *node)
{
  if (node == t->nil)
    return;

  delete_nodes(t, node->left);
  delete_nodes(t, node->right);

  free(node);
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t == NULL)
    return;

  node_t *node = t->root;

  if (node != t->nil)
  {
    delete_nodes(t, node);
  }

  free(t->nil);
  free(t);
}

//////////////////////////////////////////////////////////////////////////////////////

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED; // 무조건 레드로 추가한다

  node_t *parent = t->nil;
  node_t *cur = t->root;

  // BST 삽입 방식을 쓴다.
  while (cur != t->nil)
  {
    parent = cur;
    if (key < cur->key)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  // 새 노드의 부모 지정
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

  // 트리, 노드를 넣어준다
  rbtree_insert_fix(t, new_node);

  return new_node;
}

void rbtree_insert_fix(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *uncle;

  int is_left = (node == parent->left);
  int parent_is_left = (parent == grand_parent->left);
  node_t *uncle = parent_is_left ? grand_parent->right : grand_parent->left;

  if (parent->color == RBTREE_BLACK)
    return;

  // case1 : 부모와 삼촌이 모두 RED
  if (uncle->color == RBTREE_RED)
  {
    parent->color = RBTREE_BLACK;
    uncle->color = RBTREE_BLACK;
    grand_parent->color = RBTREE_RED;
    rbtree_insert_fixup(t, grand_parent);
    return;
  }

  // CASE 2 & 3 (왼쪽 부모)
  if (parent_is_left)
  {
    if (!is_left)
    {
      // [CASE 3]: 현재 노드가 오른쪽 자식이면 왼쪽 회전 후 case 2로 변경
      left_rotate(t, node);
      node = node->left;
      parent = node->parent;
      grand_parent = parent->parent;
    }
    // [CASE 2]: 현재 노드가 왼쪽 자식
    right_rotate(t, grand_parent);
    // 부모와 조부모의 색을 교환
    color_t temp = parent->color;
    parent->color = grand_parent->color;
    grand_parent->color = temp;
    return;
  }

  // CASE 2 & 3 (오른쪽 부모)
  if (is_left)
  {
    // [CASE 3]: 현재 노드가 왼쪽 자식이면 오른쪽 회전 후 case 2로 변경
    right_rotate(t, node);
    node = node->right;
    parent = node->parent;
    grand_parent = parent->parent;
  }

  // [CASE 2]: 현재 노드가 오른쪽 자식
  left_rotate(t, grand_parent);
  // 부모와 조부모의 색을 교환
  color_t temp = parent->color;
  parent->color = grand_parent->color;
  grand_parent->color = temp;
}

// 오른쪽 회전
void right_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_right = node->right;

  // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
  if (parent == t->root)
    t->root = node;
  else { // 1-1) 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent) {
      grand_parent->left = node;
    }
    else{
      grand_parent->right = node;
    }
  }
  node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
  parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
  node->right = parent;        // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
  node_right->parent = parent; // 3-1) 노드의 자식의 부모를 parent로 변경
  parent->left = node_right;   // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
}

// 왼쪽으로 회전하는 함수
void left_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_left = node->left;

  // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
  if (parent == t->root) {
    t->root = node;
  }
  else
  { // 1-1) 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent) {
      grand_parent->left = node;
    }
    else {
      grand_parent->right = node;
    }
  }
  node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
  parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
  node->left = parent;         // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
  parent->right = node_left;   // 3-1) 노드의 자식의 부모를 parent로 변경
  node_left->parent = parent;  // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
}

//////////////////////////////////////////////////////////////////////////////////////

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cur = t->root;

  while (cur != t->nil) {
    if (key == cur->key) {
      return cur; // key를 찾음
    } else if (key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }

  return t->nil; // key를 찾지 못함
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

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
