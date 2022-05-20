#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "list.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

int main()
{
	FileTree fileTree;

	printf("----------TEST CREATE----------\n");
	fileTree = createFileTree(strdup("root"));
	assert(fileTree.root != NULL); // check if root was created
	assert(fileTree.root->content != NULL); // check if root content was created
	// check if root folder content list was created
	assert(((FolderContent*) fileTree.root->content)->children != NULL);
	assert(fileTree.root->type == FOLDER_NODE); // check if root is a directory

	printf("\n");

	TreeNode* currentFolder = fileTree.root;

	printf("----------TEST touch----------\n");
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
	// check the content of the file
	check_added_name = strcmp(((FileContent *)currentElement->info->content)->text, "content_b");
	assert(check_added_name == 0);
	// check if the file added is the last element
	assert(currentElement->next == NULL);

	printf("***TEST TOUCH DUPLICATE FILE WITHOUT CONTENT***\n");
	touch(currentFolder, strdup("b"), 0);
	// check if the content of the file did not change
	check_added_name = strcmp(((FileContent *)currentElement->info->content)->text, "content_b");
	assert(check_added_name == 0);
	// check if no file was added
	assert(currentElement->next == NULL);

	printf("***TEST TOUCH DUPLICATE FILE WITH CONTENT***\n");
	touch(currentFolder, strdup("b"), strdup("content_b_new"));
	// check if the content of the file did not change
	check_added_name = strcmp(((FileContent *)currentElement->info->content)->text, "content_b");
	assert(check_added_name == 0);
	// check if no file was added
	assert(currentElement->next == NULL);

	printf("\n");


	printf("----------TEST mkdir----------\n");
	printf("***TEST ADD NEW DIRECTORY***\n");
	mkdir(currentFolder, strdup("A"));
	// check if the name of the added directory is "A"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// check if the added directory is the last element
	assert(currentElement->next == NULL);

	printf("***TEST ADD EXISTING DIRECTORY***\n");
	printf("Should print:\nmkdir: cannot create directory ‘A’: File exists\n");
	printf("Prints:\n");
	mkdir(currentFolder, strdup("A"));
	// check if no element was added
	assert(currentElement->next == NULL);

	printf("***TEST CHECK EXISTING CONTENTS***\n");
	currentElement = folderContents->head;
	// the first element should be file "a"
	check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// the second element should be file "b"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// the third element should be directory "A"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// check if directory "A" is the last element
	assert(currentElement->next == NULL);

	printf("\n");


	printf("----------TEST ls----------\n");
	printf("***TEST EMPTY FILE***\n");
	printf("Should print:\na: \n");
	printf("Prints:\n");
	ls(currentFolder, "a");

	printf("***TEST NON-EMPTY FILE***\n");
	printf("Should print:\nb: content_b\n");
	printf("Prints:\n");
	ls(currentFolder, "b");

	printf("***TEST EMPTY DIRECTORY***\n");
	printf("Should print:\n \n"); // CHECK TESTS TO SEE WHAT SHOULD BE PRINTED
	printf("Prints:\n");
	ls(currentFolder, "A");

	printf("***TEST NON-EMPTY, CURRENT DIRECTORY***\n");
	printf("Should print:\na\nb\nA\n");
	printf("Prints:\n");
	ls(currentFolder, 0);

	printf("***TEST INVALID ARGUMENT***\n");
	printf("Should print:\nls: cannot access 'c': No such file or directory\n");
	printf("Prints:\n");
	ls(currentFolder, "c");

	printf("\n");


	printf("----------TEST cd----------\n");
	// TODO
	printf("***TEST ADD NEW DIRECTORY***\n");
	mkdir(currentFolder, strdup("A"));
	// check if the name of the added directory is "A"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// check if the added directory is the last element
	assert(currentElement->next == NULL);

	printf("***TEST ADD EXISTING DIRECTORY***\n");
	printf("Should print:\nmkdir: cannot create directory ‘A’: File exists\n");
	printf("Prints:\n");
	mkdir(currentFolder, strdup("A"));
	// check if no element was added
	assert(currentElement->next == NULL);

	printf("***TEST CHECK EXISTING CONTENTS***\n");
	currentElement = folderContents->head;
	// the first element should be file "a"
	check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// the second element should be file "b"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// the third element should be directory "A"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// check if directory "A" is the last element
	assert(currentElement->next == NULL);

	printf("\n");


	freeTree(fileTree);

	return 0;
}
