#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

//enum is short for enumeration -> user-defined data type tht allows for the creation of a set named integer constants.
typedef enum { RBTREE_RED, RBTREE_BLACK } color_t; 

typedef int key_t; // 각 노드에 저장되는 값의 데이터 형식 정의

typedef struct node_t { //tree의 각 노드를 표현하는 구조체 
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct { //tree를 나타내는 구조체 
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif  // _RBTREE_H_
