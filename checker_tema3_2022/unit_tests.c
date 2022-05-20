#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"

int main()
{
	struct FileTree fileTree;

	printf("----------TEST CREATE----------\n");
	fileTree = createFileTree(strdup("root"));
	assert(fileTree.root != NULL); // check if root was created

	printf("\n");

	TreeNode* currentFolder = fileTree.root;

	printf("----------TEST touch 1----------\n");
	printf("***TEST TOUCH EMPTY FILE***\n");
	touch(currentFolder, strdup("a"), 0);
	// check if the name of the added file is "a"
	List* folderContents = ((struct FolderContent*)currentFolder->content)->children;
	ListNode* currentElement = folderContents->head;
	int check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// check if the file added is the only element
	assert(currentElement->next == NULL);

	printf("***TEST TOUCH FILE WITH CONTENT***\n");
	touch(currentFolder, strdup("b"), strdup("content_b"));
	// check if the name of the added file is "b"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// check if the file added is the last element
	assert(currentElement->next == NULL);

	printf("\n");

	printf("----------TEST mkdir 1----------\n");
	printf("***TEST ADD NEW DIRECTORY***\n");
	mkdir(currentFolder, strdup("A"));
	// check if the name of the added directory is "A"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// check if the added directory is the last element
	assert(currentElement->next == NULL);

	printf("***TEST ADD EXISTING DIRECTORY***\n");
	printf("Should print:\nmkdir: cannot create directory ‘<dirname>’: File exists\n");
	printf("Prints:\n");
	mkdir(currentFolder, strdup("A"));
	// check if no element was added
	assert(currentElement->next == NULL);

	printf("\n");

	printf("----------TEST touch 2----------\n");
	printf("***TEST TOUCH***\n");

	printf("\n");

	printf("----------TEST ls----------\n");
	printf("***TEST EMPTY DIRECTORY***\n");

	printf("\n");

	freeTree(fileTree);

	return 0;
}
