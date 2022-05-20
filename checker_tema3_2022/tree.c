#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

/* Creates a TreeNode* with name `treeNodeName`. */
static TreeNode* createTreeNode(char* treeNodeName) {
	TreeNode* treeNode = malloc(sizeof(*treeNode));
	treeNode->parent = NULL;
	treeNode->name = strdup(treeNodeName);
	treeNode->content = NULL;
	// the field `type` is initialized depending on the command
	return treeNode;
}

/* Creates a FileContent with content `fileContent`. */
static FileContent* createFileContent(char* fileContent) {
    FileContent* file = malloc(sizeof(*file));
	file->text = strdup(fileContent);
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
	free(((FileContent*) fileContent)->text);
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
}

void freeTree(FileTree fileTree) {
	freeTreeNode(fileTree.root);
}


void ls(TreeNode* currentNode, char* arg) {
    // TODO
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


void mkdir(TreeNode* currentNode, char* folderName) {
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


void touch(TreeNode* currentNode, char* fileName, char* fileContent) {
    // TODO
}


void cp(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}

