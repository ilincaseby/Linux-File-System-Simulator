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
	touch(currentFolder, strdup("a"), NO_ARG);
	// check if the name of the added file is "a"
	List* folderContents = ((struct FolderContent*)currentFolder->content)->children;
	ListNode* currentElement = folderContents->head;
	int check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// check if the file is empty
	check_added_name = strcmp(((FileContent *)currentElement->info->content)->text, NO_ARG);
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
	touch(currentFolder, strdup("b"), NO_ARG);
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


	// printf("----------TEST mkdir----------\n");
	// printf("***TEST ADD NEW DIRECTORY***\n");
	// mkdir(currentFolder, strdup("A"));
	// // check if the name of the added directory is "A"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "A");
	// assert(check_added_name == 0);
	// // check if the added directory is the last element
	// assert(currentElement->next == NULL);

	// printf("***TEST ADD EXISTING DIRECTORY***\n");
	// printf("Should print:\nmkdir: cannot create directory ‘A’: File exists\n");
	// printf("Prints:\n");
	// mkdir(currentFolder, strdup("A"));
	// // check if no element was added
	// assert(currentElement->next == NULL);

	// printf("***TEST CHECK EXISTING CONTENTS***\n");
	// currentElement = folderContents->head;
	// // the first element should be file "a"
	// check_added_name = strcmp(currentElement->info->name, "a");
	// assert(check_added_name == 0);
	// // the second element should be file "b"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "b");
	// assert(check_added_name == 0);
	// // the third element should be directory "A"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "A");
	// assert(check_added_name == 0);
	// // check if directory "A" is the last element
	// assert(currentElement->next == NULL);

	// printf("\n");


	printf("----------TEST ls----------\n");
	printf("***TEST INVALID ARGUMENT***\n");
	printf("Should print:\nls: cannot access 'c': No such file or directory\n");
	printf("Prints:\n");
	ls(currentFolder, "c");

	printf("***TEST EMPTY FILE***\n");
	printf("Should print:\na: \n");
	printf("Prints:\n");
	ls(currentFolder, "a");

	printf("***TEST NON-EMPTY FILE***\n");
	printf("Should print:\nb: content_b\n");
	printf("Prints:\n");
	ls(currentFolder, "b");

	// printf("***TEST EMPTY DIRECTORY***\n");
	// printf("Should print:\n \n"); // CHECK TESTS TO SEE WHAT SHOULD BE PRINTED
	// printf("Prints:\n");
	// ls(currentFolder, "A");

	printf("***TEST NON-EMPTY, CURRENT DIRECTORY***\n");
	printf("Should print:\na\nb\n");
	printf("Prints:\n");
	ls(currentFolder, NO_ARG);

	printf("\n");


	// printf("----------TEST cd----------\n");
	// printf("***TEST CHANGE TO LEVEL 1 PATH***\n");
	// cd(currentFolder, "A");
	// // check if the current directory is "A"
	// assert(strcmp(currentFolder->name, "A") == 0);
	// // check if "A" is empty
	// assert(currentFolder->content == NULL);

	// printf("***TEST CHANGE TO PARENT PATH***\n");
	// cd(currentFolder, "..");
	// // check if the current directory is "root"
	// assert(strcmp(currentFolder->name, "root") == 0);
	// // test existing contents of "root"
	// currentElement = folderContents->head;
	// // the first element should be file "a"
	// check_added_name = strcmp(currentElement->info->name, "a");
	// assert(check_added_name == 0);
	// // the second element should be file "b"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "b");
	// assert(check_added_name == 0);
	// // the third element should be directory "A"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "A");
	// assert(check_added_name == 0);
	// // check if directory "A" is the last element
	// assert(currentElement->next == NULL);

	// printf("***TEST CHANGE TO COMPOSITE PATH***\n");
	// cd(currentFolder, "A/..");
	// // check if the current directory is "root"
	// assert(strcmp(currentFolder->name, "root") == 0);
	// // test existing contents of "root"
	// currentElement = folderContents->head;
	// // the first element should be file "a"
	// check_added_name = strcmp(currentElement->info->name, "a");
	// assert(check_added_name == 0);
	// // the second element should be file "b"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "b");
	// assert(check_added_name == 0);
	// // the third element should be directory "A"
	// currentElement = currentElement->next;
	// check_added_name = strcmp(currentElement->info->name, "A");
	// assert(check_added_name == 0);
	// // check if directory "A" is the last element
	// assert(currentElement->next == NULL);

	// printf("\n");


	// printf("----------TEST tree----------\n");
	// // add a file and a directory to directory "A"
	// cd(currentFolder, "A");
	// touch(currentFolder, strdup("c"), 0);
	// mkdir(currentFolder, strdup("B"));
	// // go back to root
	// cd(currentFolder, "..");
	// printf("***TEST CURRENT PATH (root)***\n");
	// printf("Should print:\na\nb\nA\n    c\n    B\n2 directories, 3 files\n");
	// printf("Prints:\n");
	// tree(currentFolder, 0);

	// printf("***TEST LEVEL 1 PATH***\n");
	// printf("Should print:\nc\nB\n1 directories, 1 files\n");
	// printf("Prints:\n");
	// tree(currentFolder, "A");

	// printf("***TEST LEVEL 2 PATH WITH EMPTY CONTENT***\n");
	// printf("Should print:\n0 directories, 0 files\n");
	// printf("Prints:\n");
	// tree(currentFolder, "A/B");

	// printf("***TEST INVALID PATH***\n");
	// printf("Should print:\na [error opening dir]\n\n0 directories, 0 files\n");
	// printf("Prints:\n");
	// tree(currentFolder, "a");

	// printf("\n");


	// printf("----------TEST pwd----------\n");
	// printf("***TEST FROM ROOT***\n");
	// printf("Should print:\nroot\n");
	// printf("Prints:\n");
	// pwd(currentFolder);

	// printf("***TEST FROM LEVEL 1 PATH***\n");
	// cd(currentFolder, "A");
	// printf("Should print:\nroot/A\n");
	// printf("Prints:\n");
	// pwd(currentFolder);

	// printf("***TEST FROM LEVEL 2 PATH***\n");
	// cd(currentFolder, "B");
	// printf("Should print:\nroot/A/B\n");
	// printf("Prints:\n");
	// pwd(currentFolder);

	// printf("\n");


	// printf("----------TEST rmdir----------\n");
	// printf("***TEST EMPTY DIRECTORY***\n");
	// // the current folder is B; change to A
	// cd(currentFolder, "A");
	// rmdir(currentFolder, "B");
	// // check if only file c is left in directory A
	// folderContents = ((struct FolderContent*)currentFolder->content)->children;
	// currentElement = folderContents->head;
	// // the list must not be empty
	// assert(currentElement);
	// // the head must be c
	// check_added_name = strcmp(currentElement->info->name, "c");
	// // the head must be the only node
	// assert(!currentElement->next);
	// // check with ls
	// printf("Should print:\nc\n");
	// printf("Prints:\n");
	// ls(currentFolder, 0);

	// printf("***TEST NON-EMPTY DIRECTORY***\n");
	// cd(currentFolder, "root");
	// printf("Should print:\nrmdir: failed to remove 'A': Directory not empty\n");
	// printf("Prints:\n");
	// rmdir(currentFolder, "A");

	// printf("***TEST NON-EXISTENT DIRECTORY***\n");
	// cd(currentFolder, "A");
	// printf("Should print:\nrmdir: failed to remove 'B': No such file or directory\n");
	// printf("Prints:\n");
	// rmdir(currentFolder, "B");

	// printf("***TEST FILE***\n");
	// cd(currentFolder, "root");
	// printf("Should print:\nrmdir: failed to remove 'a': Not a directory\n");
	// printf("Prints:\n");
	// rmdir(currentFolder, "a");

	// printf("\n");


	// printf("----------TEST rm----------\n");
	// printf("***TEST FILE FROM root***\n");
	// // the current folder is root
	// rm(currentFolder, "a");
	// // check if root contains only file b and directory A
	// folderContents = ((struct FolderContent*)currentFolder->content)->children;
	// currentElement = folderContents->head;
	// // the list must not be empty
	// assert(currentElement);
	// // the head must be b
	// check_added_name = strcmp(currentElement->info->name, "b");
	// assert(check_added_name == 0);
	// // the next element must be A
	// currentElement = currentElement->next;
	// assert(currentElement);
	// check_added_name = strcmp(currentElement->info->name, "A");
	// assert(check_added_name == 0);
	// // the next element must be NULL
	// assert(!currentElement->next);

	// printf("***TEST DIRECTORY***\n");
	// printf("Should print:\nrm: cannot remove 'A': Is a directory\n");
	// printf("Prints:\n");
	// rm(currentFolder, "A");

	// printf("***TEST NON-EXISTENT FILE 1***\n");
	// printf("Should print:\nrm: failed to remove 'a': No such file or directory\n");
	// printf("Prints:\n");
	// rm(currentFolder, "a");

	// printf("***TEST NON-EXISTENT FILE 2***\n");
	// printf("Should print:\nrm: failed to remove 'c': No such file or directory\n");
	// printf("Prints:\n");
	// rm(currentFolder, "c");

	// printf("\n");


	// printf("----------TEST rmrec----------\n");
	// printf("***TEST FILE FROM root***\n");
	// // the current folder is root
	// rmrec(currentFolder, "b");
	// // check if root contains only directory A
	// folderContents = ((struct FolderContent*)currentFolder->content)->children;
	// currentElement = folderContents->head;
	// // the list must not be empty
	// assert(currentElement);
	// // the head must be A
	// check_added_name = strcmp(currentElement->info->name, "A");
	// assert(check_added_name == 0);
	// // the next element must be NULL
	// assert(!currentElement->next);

	// printf("***TEST DIRECTORY***\n");
	// rmrec(currentFolder, "A");
	// // check if root is empty
	// folderContents = ((struct FolderContent*)currentFolder->content)->children;
	// currentElement = folderContents->head;
	// // the list must be empty
	// assert(!currentElement);

	// printf("***TEST NON-EXISTENT RESOURCE (file 1)***\n");
	// printf("Should print:\nrmrec: failed to remove 'a': No such file or directory\n");
	// printf("Prints:\n");
	// rmrec(currentFolder, "a");

	// printf("***TEST NON-EXISTENT RESOURCE (file 2)***\n");
	// printf("Should print:\nrmrec: failed to remove 'c': No such file or directory\n");
	// printf("Prints:\n");
	// rmrec(currentFolder, "c");

	// printf("***TEST NON-EXISTENT RESOURCE (directory 1)***\n");
	// printf("Should print:\nrmrec: failed to remove 'A': No such file or directory\n");
	// printf("Prints:\n");
	// rmrec(currentFolder, "A");

	// printf("***TEST NON-EXISTENT RESOURCE (directory 2)***\n");
	// printf("Should print:\nrmrec: failed to remove 'B': No such file or directory\n");
	// printf("Prints:\n");
	// rmrec(currentFolder, "B");

	// printf("\n");


	// printf("----------TEST cp----------\n");
	// // TODO

	// printf("\n");


	// printf("----------TEST mv----------\n");
	// // TODO

	// printf("\n");


	freeTree(fileTree);
	printf("----------SUCCESS! :)----------\n");
	printf("DON'T FORGET TO CHECK MEMORY LEAKS!\n");

	return 0;
}
