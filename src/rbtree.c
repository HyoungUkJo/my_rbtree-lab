#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void left_rotate(rbtree *t, node_t *curNode){
  
  node_t *childNode = curNode->right;
  // 1. 부모의 오른쪽 내 왼쪽노드 넘겨
  curNode->right = childNode->left;
  // 2. 내 왼쪽에 있던 자식들의 부모를 curNode로
  if(childNode->left !=t->nil)
    childNode->left->parent = curNode;
  // 3. 내 부모를 바꿔
  childNode->parent = curNode->parent;
  if(curNode->parent==t->nil){
    t->root=childNode;
  }
  else{
    // 4. 부모에게 내가 자식이라 알리기
    if (curNode == curNode->parent->left)
      curNode->parent->left = childNode;
    else curNode->parent->right = childNode;
  }
  // 5. 부모의 부보는 나, 내 오른쪽은 부모 선언(자식 부모 관계 변경)
  curNode->parent = childNode;
  childNode->left = curNode;
}


void right_rotate(rbtree *t, node_t *curNode){
  node_t *childNode = curNode->left;
  // 1. 부모의 왼쪽 내 오른쪽노드 넘겨
  curNode->left = childNode->right;
  // 2. 내 오른쪽에 있던 자식들의 부모를 curNode로
  if(childNode->right !=t->nil)
    childNode->right->parent = curNode;
  // 3. 내 부모를 바꿔
  childNode->parent = curNode->parent;
  if(curNode->parent==t->nil){
    t->root=childNode;
  }
  else{
    // 4. 부모에게 내가 자식이라 알리기
    if (curNode == curNode->parent->left)
      curNode->parent->left = childNode;
    else curNode->parent->right = childNode;
  }
  // 5. 부모의 부보는 나, 내 오른쪽은 부모 선언(자식 부모 관계 변경)
  curNode->parent = childNode;
  childNode->right = curNode;
}

void rb_check(rbtree *t, node_t *curNode){

}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *newNode = malloc(sizeof(node_t));   //새로운 노드를 위한 주소 할당.
  newNode->key = key;                         // 새로운 노드의 값을 저장
  // 새로운 노드가 들어갈 위치를 찾아야함.
  node_t *curNode     = t->root;
  node_t *parentNode  = t->nil;
  node_t *sibNode     = NULL;
  // 반복문을 통해서 들어갈 수 있는 맨 마지막 노드를 찾는다.
  // 나는 nil 노드를 만들어 줄것임으로 curNode가 nil노드를 찍고 나올 수 있게 한다.
  while (curNode!=t->nil) {
    parentNode = curNode;       // 현재 노드를 부모 노드로 설정 -> 현재 노드는 nil에 다가가면 끝나기 때문에
    if (key <curNode->key)     // key가 들어갈 위치 탐색
      curNode = curNode->left;  
    else curNode = curNode->right;
  }
  if (parentNode == t->nil)   // key의 위치 최종 선정
    t->root = newNode;
  else if (key < parentNode->key)   // 
    parentNode->left = newNode;
  else parentNode->right = newNode;

  newNode->left   = t->nil;
  newNode->right  = t->nil;
  newNode->parent = parentNode;

  // 현재 노드 초기화
  curNode = newNode;
  // 루트 노드일때 예외처리
  if(curNode == t->root){
    curNode->color = RBTREE_BLACK;
    return t->root;
  }

  // ** case 분류 ** //
  // 왼쪽일때
  while(curNode->parent == RBTREE_RED){
    if (curNode->parent==curNode->parent->parent->left){
      parentNode = curNode->parent;
      sibNode = parentNode->parent->right;
        if(parentNode->color==RBTREE_RED){         // case1
          if(sibNode->color==RBTREE_RED){
            parentNode->color = RBTREE_BLACK;
            sibNode->color    = RBTREE_BLACK;
            parentNode->parent->color = RBTREE_RED;
            curNode = parentNode->parent;
          }
          else{                                   // case2
            if(curNode == parentNode->right){
              curNode = curNode->parent;
              left_rotate(t,curNode);
            }
            parentNode = curNode->parent;         //c ase 1
            parentNode->color = RBTREE_BLACK;
            parentNode->parent->color = RBTREE_RED;
            right_rotate(t,parentNode->parent);
          }
        }
    }
    else{
      parentNode = curNode->parent;
      sibNode = parentNode->parent->left;
        if(parentNode->color==RBTREE_RED){         // case1
          if(sibNode->color==RBTREE_RED){
            parentNode->color = RBTREE_BLACK;
            sibNode->color    = RBTREE_BLACK;
            parentNode->parent->color = RBTREE_RED;
            curNode = parentNode->parent;
          }
          else{                                   // case2
            if(curNode == parentNode->left){
              curNode = curNode->parent;
              left_rotate(t,curNode);
            }
            parentNode = curNode->parent;         //case 3
            parentNode->color = RBTREE_BLACK;
            parentNode->parent->color = RBTREE_RED;
            right_rotate(t,parentNode->parent);
          }
        }      
    }
  }

  //  ** 회전하는 친구 구현 ** //
  // left_Span or right_Span으로 구현


  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *findNode  = t->root;
  while(findNode!=t->nil && findNode->key!=key){  // key값이 다르고 nil이 아닐때 반복
    if (key < findNode->key)
      findNode=findNode->left;
    else 
      findNode=findNode->right;
  }

  return findNode;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find

  node_t *searchNil = t->root;
  node_t *findMin  = t->nil;

  while(searchNil != t->nil){
    searchNil = findMin;
    searchNil = searchNil->left;
  }

  return findMin;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *searchNil = t->root;
  node_t *findMin  = t->nil;

  while(searchNil != t->nil){
    searchNil = findMin;
    searchNil = searchNil->right;
  }

  return findMin;
}

void node_trance(rbtree *t, node_t *p){
  
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // successor가 뭐인지, 삭제되는 색이 무엇인지
   // 1.삭제하고 삭재 후 올 색이 뭐인지 확인
   // 2. 누구한테 extrablack이 갈 것인지
  node_t *successor = p;
  node_t *extraBlack = NULL;

  color_t successColor = successor->color;

  // 노드의 개수를 확인 -> 삭제되는 색이 무엇인지 확인
  // 삭제 후 노드 정렬

  if (p->right ==t->nil){
    //삭제될 색에 extrablack 부여
    extraBlack = p->right;
    // successor를 삭제된 노드로 대체 하는 함수
    
  }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
