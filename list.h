//
// Created by 1 on 17.11.2020.
//

#ifndef DEDLIST_LIST_H
#define DEDLIST_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <assert.h>

typedef double Element;
const Element poison = NAN;

struct Node {

    int next   = -1;
    int prev   = -1;
    Element value = poison;

};

struct List {

    int head         = 0;
    int head_free_el = 0;
    size_t size      = 0;
    size_t capacity  = 0;
    Node* data       = nullptr;
    int tail         = 0;

};

enum ErrType {

#define DEF_ERR(name, code_list_ok, code_any_place) name,

#include "ErrList.txt"

#undef DEF_ERR

};

#define ASSERT_OK ErrType my_errno = ListOk (mainList); \
                  if ( my_errno != NOERR )              \
                        exit ( my_errno );

List    ListConstruct    (List* mainList, size_t size);
void    ListDestruct     (List* mainList);
void    ListDump         (List* mainList);
int     PushHead         (List* mainList, Element value);
int     PushTail         (List* mainList, Element value);
int     PopHead          (List* mainList);
int     PopTail          (List* mainList);
int     ListInsert       (List* mainList,  size_t position, Element value);
void    DeleteElem       (List* mainList,  size_t position);
void    LogicSort        (List* mainList, size_t listSize);
int     FindValuePos     (List mainList, Element value);
Element GetValueElem     (List mainList, size_t position);
void    PushIntoFreeList (List* mainList, size_t position);
Node    GetNode          (List mainList, size_t position);
ErrType ListOk           (List* mainList);

#endif //DEDLIST_LIST_H
