all: build

build:
	gcc main.c tree.c list.c -o sd_fs

#######################DELETE BEFORE SUBMITTING#######################
# check with command: make valgrind_check
valgrind_check:
	gcc tree.c unit_tests.c list.c -o sd_fs; \
	valgrind --leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	./sd_fs

# check with command: make unit_testing
unit_testing:
	gcc tree.c unit_tests.c list.c -o sd_fs; ./sd_fs
#######################DELETE BEFORE SUBMITTING#######################

clean:
	rm sd_fs

run:
	./sd_fs
