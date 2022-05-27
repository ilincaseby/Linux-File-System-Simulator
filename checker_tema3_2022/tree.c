#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "list.h"

#define TREE_CMD_INDENT_SIZE 4
#define NO_ARG ""
#define PARENT_DIR ".."
#define NO_ARGUMENT (!strcmp(arg, NO_ARG))
#define CD_ERROR ("cd: no such file or directory:")
#define TREE_ERROR ("[error opening dir]\n")

char* my_strdup(char* str) {
	char* dup_str = malloc((strlen(str) + 1) * sizeof(char));
	memcpy(dup_str, str, strlen(str) + 1);
	return dup_str;
}

Tree_node* tree_node_create(char* tree_node_name) {
	Tree_node* tree_node = malloc(sizeof(*tree_node));
	tree_node->parent = NULL;
	tree_node->name = tree_node_name;
	tree_node->content = NULL;
	// the field `type` is initialized depending on the command
	return tree_node;
}

/* Creates a File_content with content `file_content`. */
static File_content* file_content_create(char* file_content) {
    File_content* file = malloc(sizeof(*file));
	file->text = file_content;
	return file;
}

/* Creates a Folder_content with content `folder_content`. */
static Folder_content* folder_content_create() {
    Folder_content* folder = malloc(sizeof(*folder));
	folder->children = list_create();
	return folder;
}

File_tree file_tree_create(char* root_folder_name) {
    File_tree file_tree;
	file_tree.root = tree_node_create(root_folder_name);
	// the root of a file tree is always a directory
	file_tree.root->type = FOLDER_NODE;
	file_tree.root->content = folder_content_create();
	return file_tree;
}

/* Frees the memory allocated for a file. */
static void file_content_free(File_content* file_content) {
	free(file_content->text);
	free(file_content);
}

/* Empties a directory. */
static void folder_content_empty(Folder_content* folder_content) {
	list_empty(folder_content->children);
}

/* Frees an empty directory. */
static void folder_content_free(Folder_content* folder_content) {
	list_free(folder_content->children);
	free(folder_content);
}

void tree_node_free(Tree_node* tree_node) {
	free(tree_node->name);
	if (tree_node->type == FILE_NODE) {
		file_content_free((File_content*) tree_node->content);
	} else {
		folder_content_empty((Folder_content*) tree_node->content);
		folder_content_free((Folder_content*) tree_node->content);
	}
	free(tree_node);
	tree_node = NULL;
}

void free_tree(File_tree file_tree) {
	tree_node_free(file_tree.root);
}

void touch(Tree_node* current_node, char* file_name, char* file_content) {
	List* contents_list = ((Folder_content*) current_node->content)->children;
	// check if the node is in the list
	List_node* new_node = list_find_node(contents_list, file_name);
	if (new_node == NULL) {  // the node is not in the list
		File_content* new_file_content = file_content_create(file_content);
		new_node = list_add_first(contents_list, FILE_NODE,
								  file_name, new_file_content);
		new_node->info->parent = current_node;
		return;
	}
	// the node is in the list; command does nothing
	free(file_name);
	free(file_content);
}

void ls(Tree_node* current_node, char* arg) {
	List* contents_list = ((Folder_content*) current_node->content)->children;
    // check if there is no argument
	if (NO_ARGUMENT) {
		list_print(contents_list);
		return;
	}

	// if there is an argument, check if it is valid
	List_node* list_node = list_find_node(contents_list, arg);
	if (!list_node) {
		printf("ls: cannot access '%s': No such file or directory\n", arg);
		return;
	}

	Tree_node* resource = list_node->info;
	// the argument is valid and is a file
	if (resource->type == FILE_NODE) {
		printf("%s: %s\n", resource->name, ((File_content*) resource->content)->text);
		return;
	}
	// the argument is valid and is a directory
	Folder_content* folder = resource->content;
	list_print(folder->children);
}

void mkdir(Tree_node* current_node, char* folder_name) {
	List* contents_list = ((Folder_content*) current_node->content)->children;
    List_node* folder = list_find_node(contents_list, folder_name);
	// check if the folder already exists
	if (folder) {
		printf("mkdir: cannot create directory '%s': File exists\n", folder_name);
		free(folder_name);
		return;
	}

	Folder_content* new_folder_content = folder_content_create();
	folder = list_add_first(contents_list, FOLDER_NODE, folder_name,
							new_folder_content);
	folder->info->parent = current_node;
	return;
}

/* Returns the path given by the argument `path`. */
static Tree_node* get_to_path(Tree_node* current_node, char* path,
							 char* print_arg1, char* print_arg2) {
	char* path_copy = my_strdup(path);
	char* new_dir_name = strtok(path_copy, "/");
	while (new_dir_name != NULL) {
		List* contents_list = ((Folder_content*) current_node->content)->children;
		// change to parent directory
		if (strcmp(new_dir_name, PARENT_DIR) == 0) {
			current_node = current_node->parent;
			new_dir_name = strtok(NULL, "/");
			continue;
		}

		List_node* node = list_find_node(contents_list, new_dir_name);
		// check if path exists
		if (!node) {
			printf("%s %s\n", print_arg1, print_arg2);
			current_node = NULL;
			break;
		}

		// path exists but is a file
		if (node->info->type == FILE_NODE) {
			printf("%s %s\n", print_arg1, print_arg2);
			current_node = NULL;
			break;
		}
		// path exists and is a directory
		current_node = node->info;
		new_dir_name = strtok(NULL, "/");
	}

	free(path_copy);
	return current_node;
}

Tree_node* cd(Tree_node* current_node, char* path) {
	// the `get_to_path` function also checks exception cases
	Tree_node* dir = get_to_path(current_node, path, CD_ERROR, path);
	// if the path is not valid make no change
	if (!dir) {
		return current_node;
	}
	return dir;
}

/* Prints the contents of the current directory indented by level. */
static void
print_tree_levels(Tree_node* current_node,
				  int level, int* dir_no, int* file_no) {
	List* contents_list = ((Folder_content*) current_node->content)->children;
	List_node* curr = contents_list->head;
	while (curr) {
		// print the current resource
		printf("%*s%s\n", level * TREE_CMD_INDENT_SIZE, "", curr->info->name);
		// if the resource is a directory, recursively print the contents
		if (curr->info->type == FOLDER_NODE) {
			(*dir_no)++;
			print_tree_levels(curr->info, level + 1, dir_no, file_no);
		} else {
			(*file_no)++;
		}
		curr = curr->next;
	}
}

void tree(Tree_node* current_node, char* arg) {
    int dir_no = 0, file_no = 0;
	// the `get_to_path` function also checks exception cases
	Tree_node* dir = get_to_path(current_node, arg, arg, TREE_ERROR);
	// check if the path is valid
	if (dir) {
		print_tree_levels(dir, 0, &dir_no, &file_no);
	}
	printf("%d directories, %d files\n", dir_no, file_no);
}

void pwd(Tree_node* tree_node) {
	// initialize path name with string terminator;
	char* path = my_strdup("");
	// go backwards on the path starting from `tree_node`
	while (tree_node->parent) {
		size_t old_len = strlen(path);
		// reallocate memory for the path name to fit the new level
		path = realloc(path, old_len + strlen(tree_node->name) + 2);
		// move the previous path name to the right to fit the name of the new
		// directory
		memmove(path + strlen(tree_node->name) + 1, path, old_len + 1);
		// add the level separator "/"
		memset(path, '/', 1);
		// add the directory from the current level to the beginning
		memcpy(path + 1, tree_node->name, strlen(tree_node->name));
		tree_node = tree_node->parent;
	}
	// print the absolute path
	printf("root%s\n", path);
	free(path);
}

/* This function tells me what is the type that follows to
be deleted */
int what_case(List_node* nodey) {
	if (!nodey)
		return 0;
	if (nodey->info->type == FILE_NODE)
		return -1;
	if (nodey->info->type == FOLDER_NODE)
		return 1;
	return 0;
}

/* A function used by rmrec for recursivity use */
static void recursive_rm(List_node* dir) {
	Folder_content* dir_content = (Folder_content*) dir->info->content;
	List_node* nodey = dir_content->children->head;
	while (nodey) {
		nodey = list_remove_nth_node(dir_content->children, 0);
		int cases = what_case(nodey);
		if (cases == -1) {
			list_free_node(nodey);
		} else if (cases == 1) {
			recursive_rm(nodey);
		}
		nodey = dir_content->children->head;
	}
	list_free_node(dir);
}

void rmrec(Tree_node* current_node, char* resource_name) {
    if (current_node->type == FILE_NODE)
		return;
	Folder_content* dir_content = (Folder_content*) current_node->content;
	List_node* search_var = list_find_node(dir_content->children,
										   resource_name);
	// conditions and if it is a file, then simply delete
	// it and return back, if it's a folder, recursive_rm
	// have a role here
	if (!search_var) {
		printf("rmrec: failed to remove '%s': No such file or directory\n",
			   resource_name);
		return;
	}
	int index = 0;
	search_var = dir_content->children->head;
	// take out the node contained
	while (search_var) {
		if (strcmp(resource_name, search_var->info->name) == 0)
			break;
		index++;
		search_var = search_var->next;
	}
	search_var = list_remove_nth_node(dir_content->children, index);
	// there are two case, one for file, one for directory
	if (search_var->info->type == FILE_NODE) {
		list_free_node(search_var);
		return;
	} else if (search_var->info->type == FOLDER_NODE) {
		recursive_rm(search_var);
		return;
	}
}

/* Delete a file contained by the current directory */
void rm(Tree_node* current_node, char* file_name) {
	// check if the folder is actually a file
	// not very likely but defensive
	if (current_node->type == FILE_NODE)
		return;
	// search if the file exists and it's a file
    Folder_content* what_folder_contains = (Folder_content *)
										   current_node->content;
	List_node* search_var = list_find_node(what_folder_contains->children,
										   file_name);
	// conditions
	if (!search_var) {
		printf("rm: failed to remove '%s': No such file or directory\n",
			   file_name);
		return;
	}
	if (search_var->info->type == FOLDER_NODE) {
		printf("rm: cannot remove '%s': Is a directory\n", file_name);
		return;
	} else if (search_var->info->type == FILE_NODE) {
		// actual removing proccess
		int index = 0;
		List* list_to_find_index = what_folder_contains->children;
		List_node* looking_one = list_to_find_index->head;
		// take out the file
		while (looking_one) {
			if (strcmp(file_name, looking_one->info->name) == 0)
				break;
			index++;
			looking_one = looking_one->next;
		}
		looking_one = list_remove_nth_node(list_to_find_index, index);
		// free the whole memory allocated by the file
		list_free_node(looking_one);
	}
}

void rmdir(Tree_node* current_node, char* folder_name) {
    if (current_node->type == FILE_NODE)
		return;
	Folder_content* what_folder_contains = (Folder_content*)
											current_node->content;
	List_node* search_var = list_find_node(what_folder_contains->children,
										   folder_name);
	// conditions
	if (!search_var) {
		printf("rmdir: failed to remove '%s': No such file or directory\n",
			   folder_name);
		return;
	} else if (search_var->info->type == FILE_NODE) {
		printf("rmdir: failed to remove '%s': Not a directory\n", folder_name);
		return;
	}
	Folder_content* folder = (Folder_content*) search_var->info->content;
	if (folder->children->head) {
		printf("rmdir: failed to remove '%s': Directory not empty\n",
			   folder_name);
		return;
	}
	int index = 0;
	search_var = what_folder_contains->children->head;
	// take out from the list the folder
	while (search_var) {
		if (strcmp(folder_name, search_var->info->name) == 0)
			break;
		index++;
		search_var = search_var->next;
	}
	search_var = list_remove_nth_node(what_folder_contains->children, index);
	// free the whole memory allocated
	list_free_node(search_var);
}

/* Returns the element from the source path. */
static Tree_node* get_to_source_path(char* cmd, Tree_node* current_node,
									char* src) {
	char* path_copy = my_strdup(src);
	char* new_dir_name = strtok(path_copy, "/");
	while (new_dir_name != NULL) {
		List* contents_list = ((Folder_content*)
							   current_node->content)->children;
		// change to parent directory
		if (strcmp(new_dir_name, PARENT_DIR) == 0) {
			current_node = current_node->parent;
			new_dir_name = strtok(NULL, "/");
			continue;
		}

		List_node* node = list_find_node(contents_list, new_dir_name);
		// path does not exist
		if (!node) {
			printf("%s: failed to access '%s': Not a directory\n", cmd, src);
			current_node = NULL;
			break;
		}

		// path exists and is a file
		if (node->info->type == FILE_NODE) {
			// if the file is not at the end of the path, the path is invalid
			if ((new_dir_name = strtok(NULL, "/"))) {
				printf("%s: failed to access '%s': Not a directory\n", cmd,
					   src);
				current_node = NULL;
				break;
			}

			// the path is valid
			current_node = node->info;
			break;
		}
		// path exists and is a directory
		current_node = node->info;
		new_dir_name = strtok(NULL, "/");
	}

	free(path_copy);
	// the `cp` command is invalid if the source path ends in a directory
	if (strcmp(cmd, "cp") == 0 && current_node->type == FOLDER_NODE) {
		printf("cp: -r not specified; omitting directory '%s'\n", src);
		return NULL;
	}
	// the path is valid
	return current_node;
}

/* Returns the element from the destination path. */
static Tree_node* get_to_destination_path(char* cmd, Tree_node* current_node,
										  char* dest) {
	char* path_copy = my_strdup(dest);
	char* new_res_name = strtok(path_copy, "/");
	while (new_res_name != NULL) {
		List* contents_list = ((Folder_content*)
							   current_node->content)->children;
		// change to parent directory
		if (strcmp(new_res_name, PARENT_DIR) == 0) {
			current_node = current_node->parent;
			new_res_name = strtok(NULL, "/");
			continue;
		}

		List_node* node = list_find_node(contents_list, new_res_name);
		// path does not exist
		if (!node) {
			// if `node` is not at the end of the file the path is invalid
			// make a copy of the current name
			char* new_res_name_copy = new_res_name;
			if ((new_res_name = strtok(NULL, "/"))) {
				printf("%s: failed to access '%s': Not a directory\n", cmd,
					   dest);
				current_node = NULL;
				break;
			}

			// the path is valid; create the new file
			touch(current_node, my_strdup(new_res_name_copy), NO_ARG);
			contents_list = ((Folder_content*)
							 current_node->content)->children;
			current_node = contents_list->head->info;
			break;
		}

		// path exists and is a file
		if (node->info->type == FILE_NODE) {
			// if the file is not at the end of the path, the path is invalid
			if ((new_res_name = strtok(NULL, "/"))) {
				printf("%s: failed to access '%s': Not a directory\n", cmd,
					   dest);
				current_node = NULL;
				break;
			}

			// the path is valid
			current_node = node->info;
			break;
		}
		// path exists and is a directory
		current_node = node->info;
		new_res_name = strtok(NULL, "/");
	}

	free(path_copy);
	return current_node;
}

/* Copies the content of `source` to `destination`. */
static void copy_content(Tree_node* source, Tree_node* destination) {
	if (!source || !destination) {
		return;
	}

	if (destination->type == FOLDER_NODE) {  // the destination is a directory
		Folder_content* dest_content = destination->content;

		if (source->type == FILE_NODE) {  // copy a file to a directory
			char* src_text = ((File_content*) source->content)->text;
			File_content* content_copy =
				file_content_create(my_strdup(src_text));
			List_node* new_node = list_add_first(dest_content->children,
												 source->type,
												 my_strdup(source->name),
												 content_copy);
			new_node->info->parent = destination;
		} else {  // copy a directory to a directory
			Folder_content* src_content = source->content;
			List_node* src_node = src_content->children->head;
			Folder_content* content_copy = folder_content_create();
			List_node* new_node = list_add_first(dest_content->children,
												 source->type,
												 my_strdup(source->name),
												 content_copy);
			new_node->info->parent = destination;
			Tree_node* new_destination = new_node->info;
			while (src_node) {
				copy_content(src_node->info, new_destination);
				src_node = src_node->next;
			}
		}
	} else {  // the destination is a file
		File_content* dest_content = destination->content;
		File_content* src_content = source->content;
		free(dest_content->text);
		dest_content->text = my_strdup(src_content->text);
	}
}

void cp(Tree_node* current_node, char* source, char* destination) {
	char cmd[3] = "cp";
	// the exception cases are handled by the `get_to_path` functions
    Tree_node* source_res = get_to_source_path(cmd, current_node, source);
	Tree_node* destination_res = get_to_destination_path(cmd, current_node,
														destination);
	copy_content(source_res, destination_res);
}

void mv(Tree_node* current_node, char* source, char* destination) {
	char cmd[3] = "mv";
	// the exception cases are handled by the `get_to_path` functions
    Tree_node* source_res = get_to_source_path(cmd, current_node, source);
	Tree_node* destination_res = get_to_destination_path(cmd, current_node,
														destination);
	copy_content(source_res, destination_res);
	if (source_res->type == FILE_NODE) {
		rm(source_res->parent, source_res->name);
	} else {
		rmrec(source_res->parent, source_res->name);
	}
}
