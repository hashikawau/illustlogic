
objs = \
    main.o \
    Solver.o \
    string_utils.o

all:
	cd ../test && make
	make run

run:
	make illustrationlogic
	./illustrationlogic ../test/input/50x60_01.txt
	#./illustrationlogic ../test/input/30x30_03.txt
	#./illustrationlogic ../test/input/60x90_01.txt

illustrationlogic: $(objs)
	g++ -o $@ $(objs)

%.o: %.cpp %.h
	g++ -c -g -o $@ $<

clean:
	rm -f *.o
	rm -f *.exe

