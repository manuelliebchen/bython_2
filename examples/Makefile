
.SUFFIXES: .ll .by

.PHONY = all clean

CXX = clang++
BYTHON = ../bin/bython

LIBS = ../lib/libbython-std.so

all: hello_world prime ggt simple_math quicksort 

quicksort: quicksort.ll printlist.ll random_list.ll
	${CXX} -o $@ ${LIBS} $^

prime: prime.ll
	${CXX} -o $@ ${LIBS} $^
	
ggt: ggt.ll
	${CXX} -o $@ ${LIBS} $^
	
simple_math: simple_math.ll
	${CXX} -o $@ ${LIBS} $^

hello_world: hello_world.ll
	${CXX} -o $@ ${LIBS} $^
	
%.ll: %.by
	${BYTHON} $< 1> $@

clean:
	rm -rf *.ll
