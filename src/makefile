
objs = \
    main.o \
    Solver.o

all:
	make illustrationlogic
	./illustrationlogic ../test/input/30x30_03.txt

illustrationlogic: $(objs)
	g++ -o $@ $(objs)

%.o: %.cpp %.h
	g++ -std=c++11 -c -o $@ $<

clean:
	rm -f *.o
	rm -f *.exe

