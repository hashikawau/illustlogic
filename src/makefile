
objs = \
    main.o \
    Solver.o \
    string_utils.o

all:
	make illustrationlogic
	./illustrationlogic ../test/input/60x90_01.txt

illustrationlogic: $(objs)
	g++ -o $@ $(objs)

%.o: %.cpp %.h
	g++ -c -o $@ $<

clean:
	rm -f *.o
	rm -f *.exe

