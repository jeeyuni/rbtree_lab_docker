#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  // nil 노드 생성 및 초기화
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t));
  nil_node->color = RBTREE_BLACK;
  
  // 트리의 nil 및 root 설정
  p->nil = nil_node;
  p->root = p->nil;

  return p;
}

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
void delete_nodes(rbtree *t, node_t *node) {
  if (node == t-> nil) return;

  delete_nodes(t, node->left);
  delete_nodes(t, node-> right);

  free(node);
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t == NULL) return;

  node_t *node = t -> root;

  if (node != t ->nil) {
    delete_nodes(t, node);
  }

  free(t->nil);
  free(t);
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
