#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "tree.h"

List* createList() {
    List* list = malloc(sizeof(*list));
	list->head = NULL;
	return list;
}

void freeListNode(ListNode* listNode) {
	freeTreeNode(listNode->info);
	free(listNode);
}

void freeList(List* list) {
	assert(!list->head); // check if the list is empty
	free(list);
}

/* Removes from the list the element at position `n`. */
ListNode* list_remove_nth_node(List* list, unsigned int n) {
    ListNode *prev, *curr;
	// check if the list exists and is not empty
    if (!list || !list->head) {
        return NULL;
    }
	// find the node to be removed
    curr = list->head;
    prev = NULL;
    while (n > 0) {
        prev = curr;
        curr = curr->next;
        --n;
    }

    if (prev == NULL) { // the removed node is the head of the list
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }
	curr->next = NULL;

    return curr;
}

void emptyList(List* list) {
	while (list->head) {
		ListNode* removed = list_remove_nth_node(list, 0);
		freeListNode(removed);
	}
}
