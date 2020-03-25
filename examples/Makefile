
.SUFFIXES: .ll .by

.PHONY = all clean

BYTHON = ../bin/bython

LIBS = ../lib/libbython-std.so

all: quicksort

quicksort: quicksort.by.ll printlist.by.ll
	${CXX} -o $@ ${LIBS} quicksort.by.ll printlist.by.ll

quicksort.by.ll: quicksort.by
	${BYTHON} $< 1> $<.ll
	
printlist.by.ll: printlist.by
	${BYTHON} $< 1> $<.ll

clean:
	rm -rf *.ll
