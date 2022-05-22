#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

typedef struct List_node List_node;
typedef struct List List;

struct List_node {
    Tree_node* info;
    List_node* next;
};

struct List {
    List_node* head;
};

/* Creates a list. */
List* createList();

/* Frees an empty list. */
void freeList(List* list);

/* Frees a list node. */
void freeList_node(List_node* listNode);

/* Prints the names of the files and folders contained in `list`. */
void printList(List* list);

/* Adds a new element at the end of the list. */
List_node*
list_add_first(List* list, enum Tree_nodeType type, char* name, void* content);

/* Finds the node with name `name` inside the list. */
List_node* list_find_node(List* list, char* name);

/* Removes all the elements of a list. */
void emptyList(List* list);

/* Removes from the list the element at position `n`. */
List_node* list_remove_nth_node(List* list, unsigned int n);

/* This function tells me what is the type that follows to
be deleted */
int what_case(List_node *nodey);

#endif  // LIST_H
