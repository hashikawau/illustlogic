
objs = \
    main.o \
    Solver.o

all:
	make illustrationlogic
	./illustrationlogic

illustrationlogic: $(objs)
	g++ -o $@ $(objs)

%.o: %.cpp %.h
	g++ -std=c++11 -c -o $@ $<

