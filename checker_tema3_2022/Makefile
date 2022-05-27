all: build

build:
	gcc -std=c99 main.c tree.c list.c -o sd_fs

pack:
	zip SebastianIlinca_IuliaDumitru_311CA.zip main.c tree.* list.* \
	../README.md Makefile

clean:
	rm sd_fs

run:
	./sd_fs
