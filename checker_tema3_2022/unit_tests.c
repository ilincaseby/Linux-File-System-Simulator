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
	File_tree fileTree;

	printf("----------TEST CREATE----------\n");
	fileTree = create_file_tree(strdup("root"));
	assert(fileTree.root != NULL); // check if root was created
	assert(fileTree.root->content != NULL); // check if root content was created
	// check if root folder content list was created
	assert(((Folder_content*) fileTree.root->content)->children != NULL);
	assert(fileTree.root->type == FOLDER_NODE); // check if root is a directory

	printf("\n");

	Tree_node* currentFolder = fileTree.root;

	printf("----------TEST touch----------\n");
	printf("***TEST TOUCH EMPTY FILE***\n");
	touch(currentFolder, strdup("a"), NO_ARG);
	// check if the name of the added file is "a"
	List* folderContents = ((struct Folder_content*)currentFolder->content)->children;
	List_node* currentElement = folderContents->head;
	int check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// check if the file is empty
	check_added_name = strcmp(((File_content *)currentElement->info->content)->text, NO_ARG);
	assert(check_added_name == 0);
	// check if the file added is the only element
	assert(currentElement->next == NULL);

	printf("***TEST TOUCH FILE WITH CONTENT***\n");
	touch(currentFolder, strdup("b"), strdup("content_b"));
	// check if the name of the added file is "b"
	currentElement = folderContents->head;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// check the content of the file
	check_added_name = strcmp(((File_content *)currentElement->info->content)->text, "content_b");
	assert(check_added_name == 0);
	// check if the next file is a
	currentElement = currentElement->next;
	assert(currentElement != NULL);
	check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);

	printf("***TEST TOUCH DUPLICATE FILE WITHOUT CONTENT***\n");
	touch(currentFolder, strdup("b"), NO_ARG);
	// check if the content of the file did not change
	currentElement = folderContents->head;
	check_added_name = strcmp(((File_content *)currentElement->info->content)->text, "content_b");
	assert(check_added_name == 0);
	// check if no file was added
	assert(currentElement->next->next == NULL);

	printf("***TEST TOUCH DUPLICATE FILE WITH CONTENT***\n");
	touch(currentFolder, strdup("b"), strdup("content_b_new"));
	// check if the content of the file did not change
	check_added_name = strcmp(((File_content *)currentElement->info->content)->text, "content_b");
	assert(check_added_name == 0);
	// check if no file was added
	assert(currentElement->next->next == NULL);

	printf("\n");


	printf("----------TEST mkdir----------\n");
	printf("***TEST ADD NEW DIRECTORY***\n");
	mkdir(currentFolder, strdup("A"));
	// check if the name of the added directory is "A"
	currentElement = folderContents->head;
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// // check if A is empty
	// assert(currentElement->info->content == NULL);
	// check if the added directory is the last element
	assert(currentElement->next->next->next == NULL);

	printf("***TEST ADD EXISTING DIRECTORY***\n");
	printf("Should print:\nmkdir: cannot create directory ‘A’: File exists\n");
	printf("Prints:\n");
	mkdir(currentFolder, strdup("A"));
	// check if no element was added
	assert(currentElement->next->next->next == NULL);

	printf("***TEST CHECK EXISTING CONTENTS***\n");
	currentElement = folderContents->head;
	// the first element should be file "a"
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// the second element should be file "b"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// the third element should be directory "A"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// check if directory "A" is the last element
	assert(currentElement->next == NULL);

	printf("***TEST ADD NEW DIRECTORY LEVEL 1***\n");
	currentElement = folderContents->head;
	currentFolder = currentElement->info;
	mkdir(currentFolder, strdup("C"));
	// check if the name of the added directory is "C"
	currentElement = ((Folder_content*) currentFolder->content)->children->head;
	check_added_name = strcmp(currentElement->info->name, "C");
	assert(check_added_name == 0);
	// check if the parent of the added directory is A
	check_added_name = strcmp(currentElement->info->parent->name, "A");
	assert(check_added_name == 0);

	printf("***TEST ADD NEW FILE LEVEL 1***\n");
	touch(currentFolder, strdup("file"), strdup("content_file"));
	currentElement = ((Folder_content*) currentFolder->content)->children->head;
	check_added_name = strcmp(((File_content *)currentElement->info->content)->text, "content_file");
	assert(check_added_name == 0);

	printf("\n");


	printf("----------TEST ls----------\n");
	currentFolder = fileTree.root;
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

	printf("***TEST EMPTY DIRECTORY***\n");
	currentElement = folderContents->head;
	currentFolder = currentElement->info; // this is A
	// currentFolder = ((Folder_content*) currentFolder->content)->children->head->next; // this is C
	printf("Should print:\n \n"); // CHECK TESTS TO SEE WHAT SHOULD BE PRINTED
	printf("Prints:\n");
	ls(currentFolder, "C");

	printf("***TEST NON-EMPTY, CURRENT DIRECTORY***\n");
	currentFolder = fileTree.root;
	printf("Should print:\nA\nb\na\n");
	printf("Prints:\n");
	ls(currentFolder, NO_ARG);

	printf("\n");


	printf("----------TEST cd----------\n");
	printf("***TEST CHANGE TO LEVEL 1 PATH***\n");
	currentFolder = cd(currentFolder, "A");
	// check if the current directory is "A"
	assert(strcmp(currentFolder->name, "A") == 0);
	// // check if "A" is empty
	// assert(currentFolder->content == NULL);

	printf("***TEST CHANGE TO PARENT PATH***\n");
	// current directory is A
	currentFolder = cd(currentFolder, "..");
	// check if the current directory is "root"
	assert(strcmp(currentFolder->name, "root") == 0);
	// test existing contents of "root"
	currentElement = folderContents->head;
	// the first element should be directory "A"
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// the second element should be file "b"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// the third element should be file "a"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// check if file "a" is the last element
	assert(currentElement->next == NULL);

	printf("***TEST CHANGE TO COMPOSITE PATH***\n");
	currentFolder = cd(currentFolder, "A/..");
	// check if the current directory is "root"
	assert(strcmp(currentFolder->name, "root") == 0);
	// test existing contents of "root"
	currentElement = folderContents->head;
	// the first element should be directory "A"
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// the second element should be file "b"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// the third element should be file "a"
	currentElement = currentElement->next;
	check_added_name = strcmp(currentElement->info->name, "a");
	assert(check_added_name == 0);
	// check if file "a" is the last element
	assert(currentElement->next == NULL);

	printf("***TEST CHANGE TO INVALID PATH 1 (unexistend directory)***\n");
	printf("Should print:\ncd: no such file or directory: B\n");
	printf("Prints:\n");
	currentFolder = cd(currentFolder, "B");

	printf("***TEST CHANGE TO INVALID PATH 2 (file)***\n");
	printf("Should print:\ncd: no such file or directory: b\n");
	printf("Prints:\n");
	currentFolder = cd(currentFolder, "b");

	printf("\n");


	printf("----------TEST tree----------\n");
	currentFolder = fileTree.root;
	printf("***TEST CURRENT PATH (root)***\n");
	printf("Should print:\nA\n    file\n    C\nb\na\n2 directories, 3 files\n");
	printf("Prints:\n");
	tree(currentFolder, NO_ARG);

	printf("***TEST LEVEL 1 PATH***\n");
	printf("Should print:\nfile\nC\n1 directories, 1 files\n");
	printf("Prints:\n");
	tree(currentFolder, "A");

	printf("***TEST LEVEL 2 PATH WITH EMPTY CONTENT***\n");
	printf("Should print:\n0 directories, 0 files\n");
	printf("Prints:\n");
	tree(currentFolder, "A/C");

	printf("***TEST INVALID PATH 1 (unexistent directory)***\n");
	printf("Should print:\nB [error opening dir]\n\n0 directories, 0 files\n");
	printf("Prints:\n");
	tree(currentFolder, "B");

	printf("***TEST INVALID PATH 2 (file)***\n");
	currentFolder = fileTree.root;
	printf("Should print:\na [error opening dir]\n\n0 directories, 0 files\n");
	printf("Prints:\n");
	tree(currentFolder, "a");

	printf("\n");


	printf("----------TEST pwd----------\n");
	currentFolder = fileTree.root;
	printf("***TEST FROM ROOT***\n");
	printf("Should print:\nroot\n");
	printf("Prints:\n");
	pwd(currentFolder);

	printf("***TEST FROM LEVEL 1 PATH***\n");
	currentFolder = cd(currentFolder, "A");
	printf("Should print:\nroot/A\n");
	printf("Prints:\n");
	pwd(currentFolder);

	printf("***TEST FROM LEVEL 2 PATH***\n");
	currentFolder = cd(currentFolder, "C");
	printf("Should print:\nroot/A/C\n");
	printf("Prints:\n");
	pwd(currentFolder);

	printf("\n");


	printf("----------TEST rmdir----------\n");
	printf("***TEST EMPTY DIRECTORY***\n");
	// change path to directory A
	currentFolder = fileTree.root;
	currentFolder = cd(currentFolder, "A");
	rmdir(currentFolder, "C");
	// check if only file `file` is left in directory A
	folderContents = ((struct Folder_content*)currentFolder->content)->children;
	currentElement = folderContents->head;
	// the list must not be empty
	assert(currentElement);
	// the head must be `file`
	check_added_name = strcmp(currentElement->info->name, "file");
	// the head must be the only node
	assert(!currentElement->next);
	// check with ls
	printf("Should print:\nfile\n");
	printf("Prints:\n");
	ls(currentFolder, NO_ARG);

	printf("***TEST NON-EMPTY DIRECTORY***\n");
	currentFolder = fileTree.root;
	printf("Should print:\nrmdir: failed to remove 'A': Directory not empty\n");
	printf("Prints:\n");
	rmdir(currentFolder, "A");

	printf("***TEST NON-EXISTENT DIRECTORY***\n");
	currentFolder = cd(currentFolder, "A");
	printf("Should print:\nrmdir: failed to remove 'B': No such file or directory\n");
	printf("Prints:\n");
	rmdir(currentFolder, "B");

	printf("***TEST FILE***\n");
	currentFolder = fileTree.root;
	printf("Should print:\nrmdir: failed to remove 'a': Not a directory\n");
	printf("Prints:\n");
	rmdir(currentFolder, "a");

	printf("\n");


	printf("----------TEST rm----------\n");
	printf("***TEST FILE FROM root***\n");
	// the current folder is root
	rm(currentFolder, "a");
	// check if root contains only file b and directory A
	folderContents = ((struct Folder_content*)currentFolder->content)->children;
	currentElement = folderContents->head;
	// the list must not be empty
	assert(currentElement);
	// the head must be A
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// the next element must be b
	currentElement = currentElement->next;
	assert(currentElement);
	check_added_name = strcmp(currentElement->info->name, "b");
	assert(check_added_name == 0);
	// the next element must be NULL
	assert(!currentElement->next);

	printf("***TEST DIRECTORY***\n");
	printf("Should print:\nrm: cannot remove 'A': Is a directory\n");
	printf("Prints:\n");
	rm(currentFolder, "A");

	printf("***TEST NON-EXISTENT FILE 1***\n");
	printf("Should print:\nrm: failed to remove 'a': No such file or directory\n");
	printf("Prints:\n");
	rm(currentFolder, "a");

	printf("***TEST NON-EXISTENT FILE 2***\n");
	printf("Should print:\nrm: failed to remove 'c': No such file or directory\n");
	printf("Prints:\n");
	rm(currentFolder, "c");

	printf("\n");


	printf("----------TEST rmrec----------\n");
	printf("***TEST FILE FROM root***\n");
	// the current folder is root
	rmrec(currentFolder, "b");
	// check if root contains only directory A
	folderContents = ((struct Folder_content*)currentFolder->content)->children;
	currentElement = folderContents->head;
	// the list must not be empty
	assert(currentElement);
	// the head must be A
	check_added_name = strcmp(currentElement->info->name, "A");
	assert(check_added_name == 0);
	// the next element must be NULL
	assert(!currentElement->next);

	printf("***TEST DIRECTORY***\n");
	rmrec(currentFolder, "A");
	// check if root is empty
	folderContents = ((struct Folder_content*)currentFolder->content)->children;
	currentElement = folderContents->head;
	// the list must be empty
	assert(!currentElement);

	printf("***TEST NON-EXISTENT RESOURCE (file 1)***\n");
	printf("Should print:\nrmrec: failed to remove 'a': No such file or directory\n");
	printf("Prints:\n");
	rmrec(currentFolder, "a");

	printf("***TEST NON-EXISTENT RESOURCE (file 2)***\n");
	printf("Should print:\nrmrec: failed to remove 'c': No such file or directory\n");
	printf("Prints:\n");
	rmrec(currentFolder, "c");

	printf("***TEST NON-EXISTENT RESOURCE (directory 1)***\n");
	printf("Should print:\nrmrec: failed to remove 'Q': No such file or directory\n");
	printf("Prints:\n");
	rmrec(currentFolder, "Q");

	printf("***TEST NON-EXISTENT RESOURCE (directory 2)***\n");
	printf("Should print:\nrmrec: failed to remove 'B': No such file or directory\n");
	printf("Prints:\n");
	rmrec(currentFolder, "B");

	printf("\n");


	printf("----------TEST cp----------\n");
	currentFolder = ((Folder_content*) fileTree.root->content)->children->head->info;
	rm(currentFolder, "file");
	rm(currentFolder, "d");
	rm(currentFolder, "B");
	rm(currentFolder, "c");
	currentFolder = fileTree.root;
	rm(currentFolder, "A");
	touch(currentFolder, strdup("a"), strdup("content_a"));
	touch(currentFolder, strdup("b"), strdup("content_b"));
	mkdir(currentFolder, strdup("A"));
	currentFolder = cd(currentFolder, "A");
	touch(currentFolder, strdup("c"), NO_ARG);
	mkdir(currentFolder, strdup("B"));
	touch(currentFolder, strdup("d"), strdup("content_d"));
	// create hierarchy:
	// A
	//     d - content_d
	//     B
	//     c
	// b - content_b
	// a - content_a
	// printf("***TEST COPY FROM ROOT FILE TO ROOT FILE***\n");
	// currentFolder = fileTree.root;
	// tree(currentFolder, NO_ARG);
	// cp(currentFolder, "a", "b");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that b's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "root");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_d");
	// assert(check_added_name == 0);
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that contents of `c` have not changed
	// currentElement = currentElement->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     d - content_d
	// //     B
	// //     c
	// // b - content_a
	// // a - content_a
	// printf("***TEST COPY FROM ROOT FILE TO LEVEL 1 FILE***\n");
	// currentFolder = fileTree.root;
	// cp(currentFolder, "a", "A/d");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have not changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that d's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "A");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that contents of `c` have not changed
	// currentElement = currentElement->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     d - content_a
	// //     B
	// //     c
	// // b - content_a
	// // a - content_a
	// printf("***TEST COPY TO COMPOSITE PATH USING ..***\n");
	// currentFolder = fileTree.root;
	// cp(currentFolder, "a", "A/../A/c");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have not changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that contents of `c` have changed
	// currentElement = currentElement->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that c's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "A");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     d - content_a
	// //     B
	// //     c - content_a
	// // b - content_a
	// // a - content_a
	// printf("***TEST COPY EMPTY FILE FROM COMPOSITE PATH***\n");
	// touch(currentFolder, strdup("e"), NO_ARG);
	// // A
	// //     e
	// //     d - content_a
	// //     B
	// //     c - content_a
	// // b - content_a
	// // a - content_a
	// currentFolder = fileTree.root;
	// cp(currentFolder, "A/d", "b");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that b's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "root");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that contents of `c` have not changed
	// currentElement = currentElement->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     e
	// //     d - content_a
	// //     B
	// //     c - content_a
	// // b
	// // a - content_a
	// printf("***TEST COPY FROM INVALID PATH (existent directory)***\n");
	// currentFolder = fileTree.root;
	// printf("Should print:\ncp: -r not specified; omitting directory 'A'\n");
	// printf("Prints:\n");
	// cp(currentFolder, "A", "b");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have not changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that b's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "root");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that contents of `c` have not changed
	// currentElement = currentElement->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     e
	// //     d - content_a
	// //     B
	// //     c - content_a
	// // b
	// // a - content_a
	// printf("***TEST COPY TO INVALID PATH (unexistent directory)***\n");
	// currentFolder = fileTree.root;
	// printf("Should print:\ncp: failed to access 'A/P/B': Not a directory\n");
	// printf("Prints:\n");
	// cp(currentFolder, "b", "A/P/B");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have not changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that b's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "root");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that contents of `c` have not changed
	// currentElement = currentElement->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     e
	// //     d - content_a
	// //     B
	// //     c - content_a
	// // b
	// // a - content_a
	// printf("***TEST COPY TO EMPTY DIRECTORY***\n");
	// currentFolder = fileTree.root;
	// cp(currentFolder, "b", "A/B");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have not changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that b's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "root");
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_d");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that file b was added to B
	// currentFolder = cd(currentFolder, "B");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head; // this is `b`
	// // check that b's parent is B
	// check_added_name = strcmp(currentElement->info->parent->name, "B");
	// assert(check_added_name == 0);
	// // check that b is empty
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// // check that there is no element after b
	// assert(!currentElement->next);
	// // check that contents of `c` have not changed
	// currentFolder = cd(currentFolder, ".."); // this is A
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     e
	// //     d - content_a
	// //     B
	// //         b
	// //     c - content_a
	// // b
	// // a - content_a
	// printf("***TEST COPY TO NON-EMPTY DIRECTORY***\n");
	// currentFolder = fileTree.root;
	// cp(currentFolder, "a", "A/B");
	// // check that contents of `a` have not changed
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `a`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// assert(check_added_name == 0);
	// // check that a's parent did not change
	// check_added_name = strcmp(currentElement->info->parent->name, "root");
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);
	// // check that contents of `b` have not changed
	// currentElement = folderContents->head->next; // this is `b`
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check the contents of A
	// currentFolder = cd(currentFolder, "A");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// // check that contents of `d` have not changed
	// currentElement = folderContents->head; // this is `d`
	// check_added_name = strcmp(currentElement->info->name, "d");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_d");
	// assert(check_added_name == 0);
	// // check B
	// currentElement = currentElement->next; // this is `B`
	// check_added_name = strcmp(currentElement->info->name, "B");
	// assert(check_added_name == 0);
	// // check that file a was added to B
	// currentFolder = cd(currentFolder, "B");
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head; // this is `a`
	// check_added_name = strcmp(currentElement->info->name, "a");
	// assert(check_added_name == 0);
	// // check that a's parent is B
	// check_added_name = strcmp(currentElement->info->parent->name, "B");
	// assert(check_added_name == 0);
	// // check that a is not empty
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, "content_a");
	// // check that file b did not change
	// currentElement = currentElement->next; // this is `b`
	// check_added_name = strcmp(currentElement->info->name, "b");
	// assert(check_added_name == 0);
	// // check that b is empty
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// // check that no element is after b
	// assert(!currentElement->next);
	// // check that contents of `c` have not changed
	// currentFolder = cd(currentFolder, ".."); // this is A
	// folderContents = ((struct Folder_content*)currentFolder->content)->children;
	// currentElement = folderContents->head->next->next; // this is `c`
	// check_added_name = strcmp(currentElement->info->name, "c");
	// assert(check_added_name == 0);
	// check_added_name = strcmp(((File_content*) currentElement->info->content)->text, NO_ARG);
	// assert(check_added_name == 0);
	// // check that no element was added
	// assert(!currentElement->next);

	// // A
	// //     e
	// //     d - content_a
	// //     B
	// //         a - content_a
	// //         b
	// //     c - content_a
	// // b
	// // a - content_a


	// printf("\n");


	// printf("----------TEST mv----------\n");
	// // TODO

	// printf("\n");


	free_tree(fileTree);
	printf("--------------------------SUCCESS! :)--------------------------\n");
	printf("DON'T FORGET TO CHECK PRINTS ABOVE!\n");
	printf("DON'T FORGET TO CHECK MEMORY LEAKS USING `make valgrind_check`!\n");

	return 0;
}
