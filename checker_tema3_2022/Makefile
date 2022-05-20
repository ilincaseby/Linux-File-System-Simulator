all: build

build:
	gcc main.c tree.c list.c -o sd_fs

#######################DELETE BEFORE SUBMITTING#######################
# check with command: make valgrind_check
valgrind_check:
	gcc tree.c unit_tests.c list.c -o sd_fs; valgrind ./sd_fs

unit_testing:
	gcc tree.c unit_tests.c list.c -o sd_fs; ./sd_fs
# check with command: make unit_testing
#######################DELETE BEFORE SUBMITTING#######################

clean:
	rm *.o sd_fs

run:
	./sd_fs
