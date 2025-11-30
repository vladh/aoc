RS_FILES=$(shell find . -name \*.rs -printf '%P\n')
BIN_FILES_BARE=$(RS_FILES:.rs=)
BIN_FILES=$(addprefix bin/,${BIN_FILES_BARE})

.PHONY: all clean

all: $(BIN_FILES)

clean:
	rm bin/*

bin/%: %.rs
	rustc $< -o $@
