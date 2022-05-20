#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

List* list_create() {
    List* list = malloc(sizeof(*list));
	list->head = NULL;
	return list;
}
