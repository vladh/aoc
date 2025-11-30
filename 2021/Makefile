C_FILES=$(shell find . -maxdepth 1 -name \*.c -printf '%P\n')
BIN_FILES_BARE=$(C_FILES:.c=)
BIN_FILES=$(addprefix bin/,${BIN_FILES_BARE})

.PHONY: all clean

all: $(BIN_FILES)

clean:
	rm bin/*

bin/19-1: 19-1.c
	gcc -Ofast -Wno-unused-result -ggdb --std=gnu99 $< -o $@ -lm -lcglm

bin/19-2: 19-2.c
	gcc -Ofast -Wno-unused-result -ggdb --std=gnu99 $< -o $@ -lm -lcglm

bin/%: %.c
	gcc -Ofast -Wno-unused-result -ggdb --std=gnu99 $< -o $@ -lm
