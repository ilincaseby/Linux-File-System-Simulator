#include <stdio.h>
#include <string.h>
#include "tree.h"
#define LINE_MAX_LEN 1000
#define TOKEN_MAX_LEN 300

#define LS "ls"
#define PWD "pwd"
#define TREE "tree"
#define CD "cd"
#define MKDIR "mkdir"
#define RMDIR "rmdir"
#define TOUCH "touch"
#define RM "rm"
#define RMREC "rmrec"
#define MV "mv"
#define CP "cp"

void execute_command(char *cmd, char *arg1, char *arg2) {
    printf("$ %s %s %s\n", cmd, arg1, arg2);
}

TreeNode* process_command(TreeNode* current_folder,
        char cmd[3][TOKEN_MAX_LEN], int token_count) {
    execute_command(cmd[0], cmd[1], cmd[2]);
    if (!strcmp(cmd[0], LS)) {
        ls(current_folder, cmd[1]);
    } else if (!strcmp(cmd[0], PWD)) {
        pwd(current_folder);
    } else if (!strcmp(cmd[0], TREE)) {
        tree(current_folder, cmd[1]);
    } else if (!strcmp(cmd[0], CD)) {
        current_folder = cd(current_folder, cmd[1]);
    } else if (!strcmp(cmd[0], MKDIR)) {
        mkdir(current_folder, strdup(cmd[1]));
    } else if (!strcmp(cmd[0], RMDIR)) {
        rmdir(current_folder, cmd[1]);
    } else if (!strcmp(cmd[0], RM)) {
        rm(current_folder, cmd[1]);
    } else if (!strcmp(cmd[0], RMREC)) {
        rmrec(current_folder, cmd[1]);
    } else if (!strcmp(cmd[0], TOUCH)) {
        touch(current_folder, strdup(cmd[1]), strdup(cmd[2]));
    } else if (!strcmp(cmd[0], MV)) {
        mv(current_folder, cmd[1], cmd[2]);
    } else if (!strcmp(cmd[0], CP)) {
        cp(current_folder, cmd[1], cmd[2]);
    } else {
        printf("UNRECOGNIZED COMMAND!\n");
    }
    printf("\n");
    return current_folder;
}

int main() {
    char line[LINE_MAX_LEN];
    char cmd[3][TOKEN_MAX_LEN];
    char *token;

    FileTree file_tree = create_file_tree(strdup("root"));
    TreeNode* current_folder = file_tree.root;

    while (fgets(line, sizeof(line), stdin) != NULL) {
        line[strlen(line)-1] = 0;

        cmd[0][0] = cmd[1][0] = cmd[2][0] = 0;

        int token_idx = 0;
        token = strtok(line, " ");
        while (token) {
            strcpy(cmd[token_idx], token);
            ++token_idx;

            token = strtok(NULL, " ");
        }
        current_folder = process_command(current_folder, cmd, token_idx);
    }

    free_tree(file_tree);

    return 0;
}
