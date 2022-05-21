#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."
#define FREE_CONTENT(content) ({if (strcmp(content, NO_ARG)) { \
									free(content); \
								}})
#define NO_ARGUMENT (!strcmp(arg, NO_ARG))

TreeNode* createTreeNode(char* treeNodeName) {
	TreeNode* treeNode = malloc(sizeof(*treeNode));
	treeNode->parent = NULL;
	treeNode->name = treeNodeName;
	treeNode->content = NULL;
	// the field `type` is initialized depending on the command
	return treeNode;
}

/* Creates a FileContent with content `fileContent`. */
static FileContent* createFileContent(char* fileContent) {
    FileContent* file = malloc(sizeof(*file));
	file->text = fileContent;
	return file;
}

/* Creates a FolderContent with content `folderContent`. */
static FolderContent* createFolderContent() {
    FolderContent* folder = malloc(sizeof(*folder));
	folder->children = createList();
	return folder;
}

FileTree createFileTree(char* rootFolderName) {
    FileTree fileTree;
	fileTree.root = createTreeNode(rootFolderName);
	// the root of a file tree is always a directory
	fileTree.root->type = FOLDER_NODE;
	fileTree.root->content = createFolderContent();
	return fileTree;
}

/* Frees the memory allocated for a file. */
static void freeFileContent(FileContent* fileContent) {
	// if (strcmp(fileContent->text, NO_ARG) != 0) {
	// 	free(fileContent->text);
	// }
	FREE_CONTENT(fileContent->text);
	free(fileContent);
}

/* Empties a directory. */
static void emptyFolderContent(FolderContent* folderContent) {
	emptyList(folderContent->children);
}

/* Frees an empty directory. */
static void freeFolderContent(FolderContent* folderContent) {
	freeList(folderContent->children);
	free(folderContent);
}

void freeTreeNode(TreeNode* treeNode) {
	free(treeNode->name);
	if (treeNode->type == FILE_NODE) {
		freeFileContent((FileContent*) treeNode->content);
	} else {
		emptyFolderContent((FolderContent*) treeNode->content);
		freeFolderContent((FolderContent*) treeNode->content);
	}
	free(treeNode);
	treeNode = NULL;
}

void freeTree(FileTree fileTree) {
	freeTreeNode(fileTree.root);
}

void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
	List* contentsList = ((FolderContent*) currentNode->content)->children;
	// check if the node is in the list
	ListNode* newNode = list_find_node(contentsList, fileName);
	if (newNode == NULL) { //  the node is not in the list
		FileContent* newFileContent = createFileContent(fileContent);
		list_add_first(contentsList, FILE_NODE, fileName, newFileContent);
		return;
	}
	// the node is in the list; command does nothing
	free(fileName);
	FREE_CONTENT(fileContent);
}

void ls(TreeNode* currentNode, char* arg) {
	List* contentsList = ((FolderContent*) currentNode->content)->children;
    // check if there is no argument
	if (NO_ARGUMENT) {
		printList(contentsList);
		return;
	}

	// if there is an argument, check if it is valid
	ListNode* listNode = list_find_node(contentsList, arg);
	if (!listNode) {
		printf("ls: cannot access '%s': No such file or directory\n", arg);
		return;
	}

	TreeNode* resource = listNode->info;
	// the argument is valid and is a file
	if (resource->type == FILE_NODE) {
		printf("%s: %s\n", resource->name, ((FileContent*) resource->content)->text);
		return;
	}
	// the argument is valid and is a directory
	FolderContent* folder = resource->content;
	printList(folder->children);
}

void mkdir(TreeNode* currentNode, char* folderName) {
	List* contentsList = ((FolderContent*) currentNode->content)->children;
    ListNode* folder = list_find_node(contentsList, folderName);
	// check if the folder already exists
	if (folder) {
		printf("mkdir: cannot create directory ‘%s’: File exists\n", folderName);
		return;
	}

	FolderContent* newFolderContent = createFolderContent();
	list_add_first(contentsList, FOLDER_NODE, folderName, newFolderContent);
	return;
}

void pwd(TreeNode* treeNode) {
    // TODO
}

TreeNode* cd(TreeNode* currentNode, char* path) {
    // TODO
}


void tree(TreeNode* currentNode, char* arg) {
    // TODO
}

void rmrec(TreeNode* currentNode, char* resourceName) {
    // TODO
}


void rm(TreeNode* currentNode, char* fileName) {
    // TODO
}


void rmdir(TreeNode* currentNode, char* folderName) {
    // TODO
}



void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

