
objs = \
    test.o

external_objs = \
    ../src/Solver.o \
    ../src/string_utils.o

all:
	cd ../src && make $(external_objs)
	make test
	./test

test: $(objs) $(external_objs)
	g++ -o $@ $(objs) $(external_objs)

%.o: %.cpp ../src/Solver.h
	g++ -c -g -o $@ $< -I../src/

clean:
	rm -f *.o
	rm -f *.exe
	cd ../src && make clean

