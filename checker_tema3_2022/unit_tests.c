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

	TreeNode* currentFolder = fileTree.root;

	printf("----------TEST touch----------\n");
	printf("***TEST TOUCH EMPTY FILE***\n");
	touch(currentFolder, "a", 0);
	// check if the name of the added file is "a"
	List* folderContents = ((struct FolderContent*)currentFolder->content)->children;
	int check_file_name = strcmp(folderContents->head->info->name, "a");
	assert(check_file_name == 0);
	// check if the file added is the only file
	assert(folderContents->head->next == NULL);

	printf("***TEST TOUCH FILE WITH CONTENT***\n");
	touch(currentFolder, "b", "content_b");
	// check if the name of the added file is "b"
	check_file_name = strcmp(folderContents->head->next->info->name, "b");
	assert(check_file_name == 0);
	// check if the file added is the last file
	assert(folderContents->head->next->next == NULL);

	printf("----------TEST ls----------\n");
	printf("***TEST EMPTY DIRECTORY***\n");


	freeTree(fileTree);

	return 0;
}
