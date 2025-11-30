HA_FILES=$(shell find . -maxdepth 1 -name \*.ha -printf '%P\n')
BIN_FILES_BARE=$(HA_FILES:.ha=)
BIN_FILES=$(addprefix bin/,${BIN_FILES_BARE})

.PHONY: all clean

all: $(BIN_FILES)

clean:
	rm bin/*

bin/%: %.ha
	hare build -o $@ $<
