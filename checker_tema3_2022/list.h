#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

typedef struct ListNode ListNode;
typedef struct List List;

struct ListNode {
    TreeNode* info;
    ListNode* next;
};

struct List {
    ListNode* head;
};

/* Creates a list. */
List* createList();

/* Frees an empty list. */
void freeList(List* list);

/* Frees a list node. */
void freeListNode(ListNode* listNode);

/* Empties a list. */
void emptyList(List* list);

#endif  // LIST_H
