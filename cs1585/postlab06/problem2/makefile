SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(SOURCES:%.cpp=%.o) 

COMPILER = g++
CFLAGS = -Wall --pedantic-errors -O2

program: ${OBJECTS}
	${COMPILER} ${CFLAGS} ${OBJECTS} -o program

.PHONY: debug
debug: CFLAGS = -g -Wall --pedantic-errors
debug: program

%.o: %.cpp ${HEADERS}
	${COMPILER} ${CFLAGS} -c $<

.PHONY: clean
clean:

	-@rm -f program
	-@rm -f ${OBJECTS}
