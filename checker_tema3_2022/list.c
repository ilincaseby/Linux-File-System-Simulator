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

void freeList(List* list) {
	assert(!list->head); // check if the list is empty
	free(list);
}

void freeListNode(ListNode* listNode) {
	freeTreeNode(listNode->info);
	free(listNode);
}

ListNode*
list_add_last(List* list, enum TreeNodeType type, char* name, void* content)
{
	// allocate memory for the new node and its data
	ListNode* new = malloc(sizeof(*new));
	new->info = createTreeNode(name);
	new->info->type = type;
	new->info->content = content;
	new->next = NULL;

	if (list->head == NULL) {  // the list is initially empty
		list->head = new;
		return new;
	}

	// go through the list until we reach the last node
	ListNode* curr = list->head;
	while (curr->next) {
		curr = curr->next;
	}
	// when we reach the last node, we link it to the new node
	curr->next = new;
	// return the new node so we can set the parent
	return new;
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

ListNode* list_find_node(List* list, char* name) {
	ListNode* curr = list->head;
	// check if the list is empty
	if (!curr) {
		return NULL;
	}

	// check all elements of the list
	while (curr) {
		int check_name = strcmp(name, curr->info->name);
		if (check_name == 0) { // found the node
			return curr;
		}
		// check the next node
		curr = curr->next;
	}

	// the node is not in the list
	return NULL;
}

void emptyList(List* list) {
	while (list->head) {
		ListNode* removed = list_remove_nth_node(list, 0);
		freeListNode(removed);
	}
}
