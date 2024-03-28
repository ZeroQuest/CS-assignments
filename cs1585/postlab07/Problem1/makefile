SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o) 

COMPILER = g++
CFLAGS = -Wall --pedantic-errors -O2

program: ${OBJECTS}
	${COMPILER} ${CFLAGS} ${OBJECTS} -o program

.PHONY: asan
asan: CFLAGS = -g -fsanitize=address -fno-omit-frame-pointer
asan: program

%.o: %.cpp ${HEADERS}
	${COMPILER} ${CFLAGS} -c $<

symbolizer.sh: ./symbolizer.sh

.PHONY: clean
clean:

	-@rm -f program
	-@rm -f ${OBJECTS}
