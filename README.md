# Linux File System in C[^1]

This project implements the Linux file tree system in C, along with bash commands for manipulating paths, files and directories.


## Commands implemented

1. `touch <filename> [filecontent]`

Add the file `<filename>` with content `[filecontent]` to the current path. If the `[filecontent]` argument is missing, the file is created with no content. If a file with the name `<filename>` already exists the command does nothing.

2. `ls [arg]`

List the contents of `[arg]`. If `[arg]` is a file list the content of the file. If it's a directory list the files and/or directories it contains. If `[arg]` is missing list the contents of the current directory.

3. `mkdir <dirname>`

Create an empty directory with the name `<dirname>` inside the current directory.

4. `cd <path>`

Change the current working directory to the directory located at `<path>`. The parent directory is located at path `..`.

5. `tree [path]`

Print the file/directory hierachy starting from `[path]`. If no path is specified, print the hierarchy from the current directory. On the last line print the number of directories and the number of files in the hierarchy.

6. `pwd`

Print the path to the current working directory.

7. `rmdir <dirname>`

Remove the directory specified by `<dirname>` if it exists and is empty.

8. `rm <filename>`

Remove the file specified by `<filename>` if it exists.

9. `rmrec <resourcename>`

Remove the resource specified by `<resourcename>`. If it is a file the command is equivalent to `rm <filename>`. If it is a directory the command removes it from the current path along with its contents.

10. `cp <source_path> <destination_path>`

Copy the file from `<source_path>` at `<destination_path>`. If the destination is a directory copy the file inside the directory. If the destination is an existent file replace the contents of the destination file with the contents of the source file. If the destination is a file that does not exist at the current path create a new file with the same name and content as the source file.

11. `mv <source_path> <destination_path>`

Move the file or directory from `<source_path>` at `<destination_path>`. If the source is a file the command is similar to `cp <source_path> <destination_path>` but it also removes the file from the source path. If the source is a directory move it and its contents at the destination path and remove it from the source path.


## Implementation details

The project is divided into three different sections:
- the `list` section (containing _list.c_ and _list.h_), where the functionality of a simple linked list is implemented;
- the `tree` section (containing _tree.c_ and _tree.h_), where the functionality of a file tree is implemented;
- the `main` section (containing _main.c_), where the I/O operations are handled.

### List

The fundamental structures of this section are `List` and `List_node`. The `List` structure contains only one field, a pointer to a `List_node`. This represents the head of the list. The node contains two fields: a pointer to a `Tree_node`, which will be detailed in the next section, and a pointer to another `List_node`, representing the following node. It can be seen that the list and the tree sections are dependent on each other, which is not ideal.

For the `List` structure we need a constructor (`list_create`) and a destructor (`list_free`). For the nodes we only need a destructor (`list_node_free`). There is no need for a constructor since it is sufficient to have a constructor for `Tree_node`. To manipulate a list we need a method for adding an element to the list (`list_add_first`), for removing an element (`list_remove_nth_node`), for emptying the list (`list_empty`) and for visualizing the list (`list_print`).

### Tree

In this section we have the structures `File_tree`, `Tree_node`, `File_content` and `Folder_content`. `File_tree` is similar to `List` and contains only one field of type `Tree_node*` representing the root of the tree. A `Tree_node` contains four fields: _parent_ (pointer to another `Tree_node`), _name_, _type_ (either `FILE_NODE` or `FOLDER_NODE`) and _content_ (either `File_content` or `Folder_content`, depending on the previous field). `File_content` has only one field representing the content of the file, guaranteed to be a string. `Folder_content` has also one field of type `List*`, representing the list of contents of the folder.

The external, relevant functions implemented in this section are the constructors (`tree_create` and `tree_node_create`), the destructors (`tree_free` and `tree_node_free`) and the command functions. There are also some static functions in the _tree.c_ file which act as helper functions for the external ones.

### Main

This is the central unit of the project. Here the input is read and stored in variables which are then given as parameters to the command functions implemented in _tree.c_. It is somewhat an implementation of a command line. Before exiting the program, all the allocated memory is freed.


## Notes

The biggest advantage of this homework was learning to learn with a teammate using git/GitHub. We learned a lot about the push/pull process and conflict resolution.

The given skeleton should have taken into account the use of the -std=c99 flag when compiling. (Originally, it contained the `strdup` function which is not in the C standard library. This should have been avoided by the team responsible for the homework.)

Overall, it was a very interesting homework. We never thought implementing a Linux file system could be so.. manageable! And fun! These homeworks that have a clear applicability in real life are great!


[^1]: Project completed by Sebastian Ilinca and Iulia Dumitru (311CA).
