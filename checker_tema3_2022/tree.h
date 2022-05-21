#ifndef TREE_H
#define TREE_H

// #include "list.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."

typedef struct FileContent FileContent;
typedef struct FolderContent FolderContent;
typedef struct TreeNode TreeNode;
typedef struct FileTree FileTree;
typedef struct List List;

enum TreeNodeType {
    FILE_NODE,
    FOLDER_NODE
};

struct FileContent {
    char* text;
};

struct FolderContent {
    List* children;
};

struct TreeNode {
    TreeNode* parent;
    char* name;
    enum TreeNodeType type;
    void* content;
};

struct FileTree {
    TreeNode* root;
};

/* Creates a TreeNode* with name `treeNodeName`. */
TreeNode* createTreeNode(char* treeNodeName);

/* Creates a FileTree with root `rootFolderName`. */
FileTree createFileTree(char* rootFolderName);

/* Frees the memory allocated for a TreeNode. */
void freeTreeNode(TreeNode* treeNode);

/* Frees the memory allocated for a FileTree. */
void freeTree(FileTree fileTree);

/* Adds a file with name `fileName` and content `fileContent` to the current
directory. */
void touch(TreeNode* currentNode, char* fileName, char* fileContent);

void ls(TreeNode* currentNode, char* arg);

void pwd(TreeNode* treeNode);

TreeNode* cd(TreeNode* currentNode, char* path);

void tree(TreeNode* currentNode, char* arg);

void mkdir(TreeNode* currentNode, char* folderName);

void rm(TreeNode* currentNode, char* fileName);

void rmdir(TreeNode* currentNode, char* folderName);

void rmrec(TreeNode* currentNode, char* resourceName);

void cp(TreeNode* currentNode, char* source, char* destination);

void mv(TreeNode* currentNode, char* source, char* destination);

#endif  // TREE_H
