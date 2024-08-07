#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->key = 0;
  p->nil->parent = NULL;
  p->nil->left = NULL;
  p->nil->right = NULL;
  p->root = p->nil;
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
  while(curNode->parent->color == RBTREE_RED){
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
            parentNode = curNode->parent;         //case 3
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
              right_rotate(t,curNode);
            }
            parentNode = curNode->parent;         //case 3
            parentNode->color = RBTREE_BLACK;
            parentNode->parent->color = RBTREE_RED;
            left_rotate(t,parentNode->parent);
          }
        }      
    }
  }
  t->root->color = RBTREE_BLACK;

  //  ** 회전하는 친구 구현 ** //
  // left_Span or right_Span으로 구현


  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *findNode  = t->root;
  while(findNode!=t->nil && findNode->key!=key){  // key값이 다르고 nil이 아닐때 반복
    // printf("%d",findNode->key); 
    if (key < findNode->key)
      findNode=findNode->left;
    else 
      findNode=findNode->right;
  }
  if(findNode == t->nil){
    return NULL;
  }

  return findNode;
}

node_t *find_pre(rbtree* t, node_t *n){
  node_t *searchNil = n;
  node_t *findMin  = n;

  if(n->right != t->nil){
    while(searchNil != t->nil){
      findMin = searchNil;
      searchNil = searchNil->right;
    }
  }
  return findMin;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find

  node_t *searchNil = t->root;
  node_t *findMin  = t->root;

  if(searchNil->left != t->nil){
    while(searchNil != t->nil){
      findMin = searchNil;
      searchNil = searchNil->left;
    }  
  }
  else {
    searchNil=searchNil->right;
    while(searchNil != t->nil){
      searchNil = findMin;
      searchNil = searchNil->left;
    }
  }

  return findMin;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *searchNil = t->root;
  node_t *findMax  = t->root;


  if(searchNil->right != t->nil){
    while(searchNil != t->nil){
      findMax = searchNil;
      searchNil = searchNil->right;
    }  
  }
  else {
    searchNil=searchNil->left;
    while(searchNil != t->nil){
      searchNil = findMax;
      searchNil = searchNil->right;
    }
  }
  return findMax;
}

// 내 위치에 다른노드를 선언 내 부모 관계를 교체
void node_trance(rbtree *t, node_t *p, node_t * chgNode){
  if (p->parent == t->nil){
    t->root = chgNode;
  }
  // 내가 왼쪽일때
  else if (p == p->parent->left){
    p->parent->left = chgNode;
  }
  else p->parent->right = chgNode;

  chgNode->parent = p->parent;
}

void delete_fix(rbtree *t, node_t *extraBlack){
  node_t *ciblingNode = NULL;
  while (extraBlack != t->root && extraBlack->color==RBTREE_BLACK){
    if (extraBlack == extraBlack->parent->left){
      ciblingNode = extraBlack->parent->right;
      //case1.
      if(ciblingNode->color == RBTREE_RED){
        extraBlack->parent->color = RBTREE_RED;
        ciblingNode->color        = RBTREE_BLACK;
        left_rotate(t,extraBlack->parent);
        ciblingNode = extraBlack->parent->right;
      }
      //case2
      if(ciblingNode->left->color==RBTREE_BLACK && 
        ciblingNode->right->color==RBTREE_BLACK){
        ciblingNode->color=RBTREE_RED;
        extraBlack = extraBlack->parent;
      }
      else{
        //case3
        if(ciblingNode->left->color==RBTREE_RED){
          ciblingNode->color        = RBTREE_RED;
          ciblingNode->left->color  = RBTREE_BLACK;
          right_rotate(t,ciblingNode);
          ciblingNode = ciblingNode->parent;
        }
          //case4
        ciblingNode->color = ciblingNode->parent->color;
        ciblingNode->right->color = RBTREE_BLACK;
        extraBlack->parent->color = RBTREE_BLACK;
        left_rotate(t,extraBlack->parent);
        extraBlack = t->root;
      }
    }
    else {  // 오른쪽에 extraBlack이 있다면
      ciblingNode = extraBlack->parent->left;
      //case1.
      if(ciblingNode->color == RBTREE_RED){
        extraBlack->parent->color = RBTREE_RED;
        ciblingNode->color        = RBTREE_BLACK;
        right_rotate(t,extraBlack->parent);
        ciblingNode = extraBlack->parent->left;
      }
      //case2
      if(ciblingNode->left->color==RBTREE_BLACK && 
          ciblingNode->right->color==RBTREE_BLACK){
        ciblingNode->color=RBTREE_RED;
        extraBlack = extraBlack->parent;
      }
      else{
        //case3
        if(ciblingNode->right->color==RBTREE_RED){
          ciblingNode->color        = RBTREE_RED;
          ciblingNode->right->color  = RBTREE_BLACK;
          left_rotate(t,ciblingNode);
          ciblingNode = ciblingNode->parent;
        }
          //case4
        ciblingNode->color = ciblingNode->parent->color;
        ciblingNode->left->color = RBTREE_BLACK;
        extraBlack->parent->color = RBTREE_BLACK;
        right_rotate(t,extraBlack->parent);
        extraBlack = t->root;
      }      
    }
  }
  //위의 반복문이 완료 되면 extrablack을 해결한것임.
  extraBlack->color=RBTREE_BLACK;
  
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  // successor가 뭐인지, 삭제되는 색이 무엇인지
   // 1.삭제하고 삭재 후 올 색이 뭐인지 확인
   // 2. 누구한테 extrablack이 갈 것인지
  node_t *successor = p;
  node_t *extraBlack = NULL;

  color_t delColor = successor->color;

  // 노드의 개수를 확인 -> 삭제되는 색이 무엇인지 확인
  // 삭제 후 노드 정렬

  if (p->right ==t->nil){
    //삭제될 색에 extrablack 부여 바꾸기 전에 부여해야 참조할 수 있기때문
    extraBlack = p->left;
    // successor를 삭제된 노드로 대체 하는 함수
    node_trance(t,p,p->left);
  }
  else if(p->left == t->nil){
    extraBlack = p->right;
    node_trance(t,p,p->right);
  }
  else{ // 노드가 두개일때
    // 전임자를 succesor로
    successor = find_pre(t,p->left);
    // successor의 색이 없어지니까 색 갱신
    delColor = successor->color;
    // 왼쪽 노드가 있다면 왼쪽 노드가 올라올 수 있게 없어도 nil이 올라옴
    extraBlack = successor->left;
    if(successor != p->left){ 
      // successor가 p의 왼쪽이 아니라 더 깊이 있는 노드였다면
      // 내 왼쪽에 있는 친구들을 내위치로~
      node_trance(t,successor,successor->left);
      successor->left = p->left;
      p->left->parent = successor;
      // extraBlack->parent = successor->left;
    }
    else 
      extraBlack->parent = successor;
      //successor을 내 위치로
    node_trance(t,p,successor);
    successor->right = p->right;
    p->right->parent = successor;
    successor->color = p->color;

  }
  if (delColor==RBTREE_BLACK){
    //case구별할 함수 호출
    delete_fix(t,extraBlack);
  }

  return 0;
}

void min_array(const rbtree *t, node_t *node, key_t *arr, int* size, size_t n){
  if(node != t->nil && *size<n ){
    min_array(t,node->left,arr,size,n);
    if(*size < n){
      arr[*size] = node->key;
      (*size)++;
      min_array(t,node->right,arr,size,n);
    }
  }
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *node = t->root;
  int i = 0;
  int * size = &i;
  min_array(t,node,arr,size,n);

  return 0;
}
