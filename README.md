# Linux File System in C

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

## Notes

The biggest advantage of this homework was learning to learn with a teammate using git/GitHub. We learned a lot about the push/pull process and conflict resolution.

The given skeleton should have taken into account the use of the -std=c99 flag when compiling. (Originally, it contained the `strdup` function which is not in the C standard library. This should have been avoided by the team responsible for the homework.)

Overall, it was a very interesting homework. We never thought implementing a Linux file system could be so.. manageable! And fun! These homeworks that have a clear applicability in real life are great!
