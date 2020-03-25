
.SUFFIXES: .ll .by

.PHONY = all clean

CXX = clang++
BYTHON = ../bin/bython

LIBS = ../lib/libbython-std.so

all: quicksort

quicksort: quicksort.ll printlist.ll random_list.ll
	${CXX} -o $@ ${LIBS} *.ll
	
%.ll: %.by
	${BYTHON} $< 1> $@

clean:
	rm -rf *.ll
