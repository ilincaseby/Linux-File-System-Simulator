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
#define CD_ERROR ("cd: no such file or directory:")
#define TREE_ERROR ("[error opening dir]\n")

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
	if (newNode == NULL) {  // the node is not in the list
		FileContent* newFileContent = createFileContent(fileContent);
		newNode = list_add_first(contentsList, FILE_NODE, fileName, newFileContent);
		newNode->info->parent = currentNode;
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
		printf("mkdir: cannot create directory '%s’: File exists\n", folderName);
		free(folderName);
		return;
	}

	FolderContent* newFolderContent = createFolderContent();
	folder = list_add_first(contentsList, FOLDER_NODE, folderName,
						   newFolderContent);
	folder->info->parent = currentNode;
	return;
}

/* Returns the path given by the argument `path`. */
static TreeNode* get_to_path(TreeNode* currentNode, char* path,
							 char* print_arg1, char* print_arg2) {
	char* path_copy = strdup(path);
	char* new_dir_name = strtok(path_copy, "/");
	while (new_dir_name != NULL) {
		List* contentsList = ((FolderContent*) currentNode->content)->children;
		// change to parent directory
		if (strcmp(new_dir_name, PARENT_DIR) == 0) {
			currentNode = currentNode->parent;
			new_dir_name = strtok(NULL, "/");
			continue;
		}

		ListNode* node = list_find_node(contentsList, new_dir_name);
		// check if path exists
		if (!node) {
			printf("%s %s\n", print_arg1, print_arg2);
			currentNode = NULL;
			break;
		}

		// path exists but is a file
		if (node->info->type == FILE_NODE) {
			printf("%s %s\n", print_arg1, print_arg2);
			currentNode = NULL;
			break;
		}
		// path exists and is a directory
		currentNode = node->info;
		new_dir_name = strtok(NULL, "/");
	}

	free(path_copy);
	return currentNode;
}

TreeNode* cd(TreeNode* currentNode, char* path) {
	// the `get_to_path` function also checks exception cases
	TreeNode* dir = get_to_path(currentNode, path, CD_ERROR, path);
	// if the path is not valid make no change
	if (!dir) {
		return currentNode;
	}
	return dir;
}

/* Prints the contents of the current directory indented by level. */
static void
printTreeLevels(TreeNode* currentNode, int level, int* dir_no, int* file_no) {
	List* contentsList = ((FolderContent*) currentNode->content)->children;
	ListNode* curr = contentsList->head;
	while (curr) {
		// print the current resource
		printf("%*s%s\n", level * TREE_CMD_INDENT_SIZE, "", curr->info->name);
		// if the resource is a directory, recursively print the contents
		if (curr->info->type == FOLDER_NODE) {
			(*dir_no)++;
			printTreeLevels(curr->info, level + 1, dir_no, file_no);
		} else {
			(*file_no)++;
		}
		curr = curr->next;
	}
}

void tree(TreeNode* currentNode, char* arg) {
    int dir_no = 0, file_no = 0;
	// the `get_to_path` function also checks exception cases
	TreeNode* dir = get_to_path(currentNode, arg, arg, TREE_ERROR);
	// check if the path is valid
	if (dir) {
		printTreeLevels(dir, 0, &dir_no, &file_no);
	}
	printf("%d directories, %d files\n", dir_no, file_no);
}

void pwd(TreeNode* treeNode) {
	// initialize path name with string terminator;
	char* path = strdup("");
	// go backwards on the path starting from `treeNode`
	while (treeNode->parent) {
		size_t old_len = strlen(path);
		// reallocate memory for the path name to fit the new level
		path = realloc(path, old_len + strlen(treeNode->name) + 2);
		// move the previous path name to the right to fit the name of the new
		// directory
		memmove(path + strlen(treeNode->name) + 1, path, old_len + 1);
		// add the level separator "/"
		memset(path, '/', 1);
		// add the directory from the current level to the beginning
		memcpy(path + 1, treeNode->name, strlen(treeNode->name));
		treeNode = treeNode->parent;
	}
	// print the absolute path
	printf("root%s\n", path);
	free(path);
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

/* Returns the element from the source path. */
static TreeNode* get_to_source_path(char* cmd, TreeNode* currentNode,
									char* src) {
	char* path_copy = strdup(src);
	char* new_dir_name = strtok(path_copy, "/");
	while (new_dir_name != NULL) {
		List* contentsList = ((FolderContent*) currentNode->content)->children;
		// change to parent directory
		if (strcmp(new_dir_name, PARENT_DIR) == 0) {
			currentNode = currentNode->parent;
			new_dir_name = strtok(NULL, "/");
			continue;
		}

		ListNode* node = list_find_node(contentsList, new_dir_name);
		// path does not exist
		if (!node) {
			printf("%s: failed to access '%s': Not a directory\n", cmd, src);
			currentNode = NULL;
			break;
		}

		// path exists and is a file
		if (node->info->type == FILE_NODE) {
			// if the file is not at the end of the path, the path is invalid
			if ((new_dir_name = strtok(NULL, "/"))) {
				printf("%s: failed to access '%s': Not a directory\n", cmd,
					   src);
				currentNode = NULL;
				break;
			}

			// the path is valid
			currentNode = node->info;
			break;
		}
		// path exists and is a directory
		currentNode = node->info;
		new_dir_name = strtok(NULL, "/");
	}

	free(path_copy);
	// the `cp` command is invalid if the source path ends in a directory
	if (strcmp(cmd, "cp") == 0 && currentNode->type == FOLDER_NODE) {
		printf("cp: -r not specified; omitting directory '%s'\n", src);
		return NULL;
	}
	// the path is valid
	return currentNode;
}

/* Returns the element from the destination path. */
static TreeNode* get_to_destination_path(char* cmd, TreeNode* currentNode,
										 char* dest) {
	char* path_copy = strdup(dest);
	char* new_res_name = strtok(path_copy, "/");
	while (new_res_name != NULL) {
		List* contentsList = ((FolderContent*) currentNode->content)->children;
		// change to parent directory
		if (strcmp(new_res_name, PARENT_DIR) == 0) {
			currentNode = currentNode->parent;
			new_res_name = strtok(NULL, "/");
			continue;
		}

		ListNode* node = list_find_node(contentsList, new_res_name);
		// path does not exist
		if (!node) {
			// if `node` is not at the end of the file the path is invalid
			// make a copy of the current name
			char* new_res_name_copy = new_res_name;
			if ((new_res_name = strtok(NULL, "/"))) {
				printf("%s: failed to access '%s': Not a directory\n", cmd,
					   dest);
				currentNode = NULL;
				break;
			}

			// the path is valid; create the new file
			touch(currentNode, strdup(new_res_name_copy), NO_ARG);
			contentsList = ((FolderContent*) currentNode->content)->children;
			currentNode = contentsList->head->info;
			break;
		}

		// path exists and is a file
		if (node->info->type == FILE_NODE) {
			// if the file is not at the end of the path, the path is invalid
			if ((new_res_name = strtok(NULL, "/"))) {
				printf("%s: failed to access '%s': Not a directory\n", cmd,
					   dest);
				currentNode = NULL;
				break;
			}

			// the path is valid
			currentNode = node->info;
			break;
		}
		// path exists and is a directory
		currentNode = node->info;
		new_res_name = strtok(NULL, "/");
	}

	free(path_copy);
	return currentNode;
}


void cp(TreeNode* currentNode, char* source, char* destination) {
	char cmd[3] = "cp";
    TreeNode* source_file = get_to_source_path(cmd, currentNode, source);
	TreeNode* destination_file = get_to_destination_path(cmd, currentNode,
														 destination);
}

void mv(TreeNode* currentNode, char* source, char* destination) {
    // TODO
}
