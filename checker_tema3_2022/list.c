#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"
#include "tree.h"

List* list_create() {
    List* list = malloc(sizeof(*list));
	list->head = NULL;
	return list;
}

void list_free(List* list) {
	assert(!list->head);  // check if the list is empty
	free(list);
}

void list_free_node(List_node* listNode) {
	tree_node_free(listNode->info);
	free(listNode);
}

void list_print(List* list) {
	// check if the list exists
	if (!list) {
		return;
	}

	// if the list is empty print nothing
	if (!list->head) {
		printf("\n");
		return;
	}

	// go through each element of the list and print its name
	List_node* curr = list->head;
	while (curr) {
		printf("%s\n", curr->info->name);
		curr = curr->next;
	}
}

List_node*
list_add_first(List* list, enum Tree_nodeType type, char* name, void* content)
{
	// allocate memory for the new node and its data
	List_node* new = malloc(sizeof(*new));
	new->info = tree_node_create(name);
	new->info->type = type;
	new->info->content = content;

	if (list->head == NULL) {  // the list is initially empty
		list->head = new;
		new->next = NULL;
		return new;
	}

	// replace the head with the new node
	new->next = list->head;
	list->head = new;

	// return the node so we can set the parent
	return new;
}

/* Removes from the list the element at position `n`. */
List_node* list_remove_nth_node(List* list, unsigned int n) {
    List_node *prev, *curr;
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

    if (prev == NULL) {  // the removed node is the head of the list
        list->head = curr->next;
    } else {
        prev->next = curr->next;
    }
	curr->next = NULL;

    return curr;
}

List_node* list_find_node(List* list, char* name) {
	List_node* curr = list->head;
	// check if the list is empty
	if (!curr) {
		return NULL;
	}

	// check all elements of the list
	while (curr) {
		int check_name = strcmp(name, curr->info->name);
		if (check_name == 0) {  // found the node
			return curr;
		}
		// check the next node
		curr = curr->next;
	}

	// the node is not in the list
	return NULL;
}

void list_empty(List* list) {
	while (list->head) {
		List_node* removed = list_remove_nth_node(list, 0);
		list_free_node(removed);
	}
}
