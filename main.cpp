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
                  if ( my_errno ) \
                        exit ( my_errno );

List    ListConstruct    (List** mainList, size_t size);
void    ListDestruct     (List* mainList);
void    ListDump         (List  mainList);
int     PushHead         (List* mainList, Element value);
int     PushTail         (List* mainList, Element value);
int     PopHead          (List* mainList);
int     PopTail          (List* mainList);
int     ListInsert       (List* mainList,  size_t position, Element value);
void    DeleteElem       (List* mainList,  size_t position);
void    LogicSort        (List** mainList, size_t listSize);
int     FindValuePos     (List mainList, Element value);
Element GetValueElem     (List mainList, size_t position);
void    PushIntoFreeList (List* mainList, size_t position);
ErrType ListOk           (List* mainList);

int main () {

    List *mainList = nullptr;

    size_t list_size = 10;

    ListConstruct (&mainList, list_size);

    PushTail (mainList, 20);
    PushHead (mainList, 112);

    ListInsert (mainList, 2, 40);

    LogicSort (&mainList, list_size);

    ListDump      (*mainList);
    ListDestruct  (mainList);

    return 0;

}

void DeleteElem (List* mainList, size_t position) {

    ASSERT_OK

    if ( mainList->data[position].prev != 0 )
        mainList->data[mainList->data[position].prev].next = mainList->data[position].next;
    else
        mainList->head = mainList->data[position].next;

    if ( mainList->data[position].next != 0 )
        mainList->data[mainList->data[position].next].prev = mainList->data[position].prev;
    else
        mainList->tail = mainList->data[position].prev;

    mainList->data[position].value = poison;
    mainList->data[position].next = mainList->head_free_el;
    mainList->data[position].prev = 0;

    PushIntoFreeList (mainList, position);

    mainList->capacity--;

}

int ListInsert (List* mainList, size_t position, Element value) {

    ASSERT_OK

    int new_physical_position = mainList->head_free_el;

    mainList->head_free_el = mainList->data[mainList->head_free_el].next;

    if ( mainList->head_free_el != 0 )
        mainList->data[mainList->head_free_el].prev = 0;

    mainList->data[new_physical_position].next = mainList->data[position].next;
    mainList->data[new_physical_position].prev = position;
    mainList->data[position].next = new_physical_position;

    if (mainList->data[new_physical_position].next != 0)
        mainList->data[mainList->data[new_physical_position].next].prev = new_physical_position;
    else
        mainList->tail = new_physical_position;

    mainList->data[new_physical_position].value = value;

    mainList->capacity++;

    return new_physical_position;

}

int PopHead (List* mainList) {

    ASSERT_OK

    if (mainList->head == mainList->tail) //if there is only one element
        mainList->tail = 0;

    int new_head_index = mainList->data[mainList->head].next;

    mainList->data[new_head_index].prev = 0;

    /*mainList->data[mainList->head].prev = 0;
    mainList->data[mainList->head].next = 0;
    mainList->data[mainList->head].value = poison;*/

    mainList->data[mainList->head].prev = 0;
    mainList->data[mainList->head].value = poison;

    PushIntoFreeList (mainList, mainList->head);

    mainList->head = new_head_index;

    mainList->capacity--;

    return new_head_index;

}

int PopTail (List* mainList) {

    ASSERT_OK

    if (mainList->head == mainList->tail) //if there is only one element
        mainList->head = 0;

    int new_tail_index = mainList->data[mainList->tail].prev;

    mainList->data[new_tail_index].next = 0;

    mainList->data[mainList->tail].prev = 0;
    mainList->data[mainList->tail].value = poison;

    PushIntoFreeList (mainList, mainList->tail);

    mainList->tail = new_tail_index;

    mainList->capacity--;

    return new_tail_index;

}

int PushTail (List* mainList, Element value) {

    ASSERT_OK

    int new_Pos = mainList->head_free_el;
    mainList->head_free_el = mainList->data[mainList->head_free_el].next;

    if ( mainList->data[new_Pos].next != 0)
        mainList->data[mainList->data[new_Pos].next].prev = 0;


    if ( mainList->tail != 0 ) { //if there is tail
        mainList->data[new_Pos].prev = mainList->tail;
        mainList->data[mainList->tail].next = new_Pos;
    } else {
        mainList->data[new_Pos].prev = 0;
    }

    if ( mainList->head == 0 ) //if there is none head
        mainList->head = new_Pos;

    mainList->data[new_Pos].value = value;
    mainList->data[new_Pos].next = 0;
    mainList->tail = new_Pos;

    mainList->capacity++;

    return new_Pos;

}

int PushHead (List* mainList, Element value) {

    ASSERT_OK

    int new_Pos = mainList->head_free_el;
    mainList->head_free_el = mainList->data[mainList->head_free_el].next;

    if ( mainList->data[new_Pos].next != 0)
        mainList->data[mainList->data[new_Pos].next].prev = 0;


    if ( mainList->head != 0 ) { //if there is head
        mainList->data[new_Pos].next = mainList->head;
        mainList->data[mainList->head].prev = new_Pos;
    } else {
        mainList->data[new_Pos].next = 0;
    }

    if ( mainList->tail == 0 ) //if there is none tail
        mainList->tail = new_Pos;

    mainList->data[new_Pos].value = value;
    mainList->data[new_Pos].prev = 0;
    mainList->head = new_Pos;

    mainList->capacity++;

    return new_Pos;

}

void ListFillPoison (Node* data, size_t size) {

    for (size_t i = 1; i < size; i++) {

        data[i].prev = i - 1;
        data[i].next = (i + 1) % size;
        data[i].value = poison;

    }

}

List ListConstruct (List** mainList, size_t size) {

    *mainList = (List*)calloc(1, sizeof(Node));

    (*mainList)->size = size;
    (*mainList)->capacity = 0;
    (*mainList)->head = (*mainList)->tail = 0;

    (*mainList)->data = (Node*)calloc(size, sizeof(Node));

    ListFillPoison ((*mainList)->data, size);

    (*mainList)->head_free_el = 1;

    return **mainList;

}

void ListDestruct (List* mainList) {

    ListOk (mainList);

    mainList->size = 0;
    mainList->capacity = 0;
    free ( mainList->data );
    mainList->data = nullptr;

    mainList->head_free_el = mainList->head = mainList->tail = 0;

    mainList = nullptr;

}

void MakeListGraph (List mainList) {

    FILE *graph = fopen ("graph.dot", "w");

    fprintf (graph, "digraph List {\n"
                    "node [shape=\"box\"]\n");

    /*for (;; i = mainList.data[i].next) {

        if ( i == -1 ) break;

        fprintf (graph, "\"box%zu\" [shape = \"record\", label = \"{%zu|%lg|{%d|%d}}\"]", i, i, mainList.data[i].value, mainList.data[i].prev, mainList.data[i].next);

        if ( mainList.data[i].prev != -1 ) {

           fprintf (graph, "\"box%zu\" -> \"box%zu\";", mainList.data[i].prev, i);

        }

    }*/

    for (size_t i = 0; i < mainList.size; i++) {

        fprintf (graph, "\"box%zu\" [shape = \"record\", label = \"{%zu|%lg|{%d|%d}}\"]", i, i, mainList.data[i].value, mainList.data[i].prev, mainList.data[i].next);

        if ( mainList.data[i].prev != 0 ) {

            fprintf (graph, "\"box%zu\" -> \"box%zu\";", mainList.data[i].prev, i);

        }

    }

    fprintf (graph, "}\n");

    fclose (graph);

    system ("dot -Tpng graph.dot -o graph.png");

}

void ListDump (List mainList) {
    //вынести в один fprintf
    FILE* html_output = fopen ("dump.html", "w");

    MakeListGraph (mainList);

    fprintf (html_output, "<!DOCTYPE html>\n"
                          "<html lang=\"en\">\n"
                          "<head>\n"
                          "\t<meta charset=\"UTF-8\">\n"
                          "\t<title>List dump</title>\n"
                          "<link href=\"https://fonts.googleapis.com/css2?family=Open+Sans&display=swap\" rel=\"stylesheet\">"
                          "<link rel=\"stylesheet\" href=\"style.css\">\n"
                          "</head>\n"
                          "<body>\n");

    fprintf (html_output, "<div class = \"container\">");

    fprintf (html_output, "<ul>"
                          "<li>Head index = %d</li>"
                          "<li>Tail index = %d</li>"
                          "<li>Head index of free element's list = %d</li>"
                          "</ul>", mainList.head, mainList.tail, mainList.head_free_el);

    fprintf (html_output, "<span> List size = %zu </span> <br>\n <span> List capacity = %zu </span><br>\n", mainList.size, mainList.capacity);

    fprintf (html_output, "<img src = \"graph.png\">");

    fprintf (html_output, "</div>\n"
                          "</body>\n"
                          "</html>");


    fclose (html_output);

}

void LogicSort (List** mainList, size_t listSize) {

    List* tempList = nullptr;
    ListConstruct (&tempList, listSize);

    for (size_t i = (*mainList)->head; i != 0; i = (*mainList)->data[i].next) {

        PushTail (tempList, (*mainList)->data[i].value);

    }

    List* oldListPointer = *mainList;

    *mainList = tempList;

    ListDestruct (oldListPointer);

}

int FindValuePos (List mainList, Element value) {

    for (size_t i = mainList.head; i != 0;i = mainList.data[i].next) {

        if ( mainList.data[i].value == value ) return i;

    }

    return 0;

}

Element GetValueElem (List mainList, size_t position) {

    for (size_t i = mainList.head; i != 0; i = mainList.data[i].next) {

        if (i == position) return mainList.data[i].value;

    }

    return NAN;

}

void PushIntoFreeList (List* mainList, size_t position) {

    if (mainList->head_free_el != 0) //if there is a head
        mainList->data[mainList->head_free_el].prev = position;

    mainList->head_free_el = position;

}

ErrType ListOk (List* mainList) {

#define DEF_ERR(name, code_list_ok, code_any_place) code_list_ok

#include "ErrList.txt"

#undef DEF_ERR

    return NOERR;

}
