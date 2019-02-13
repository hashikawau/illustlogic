
objs = \
    main.obj \
    iraroji.obj
    #main.o \
    #Solver.o \
    #string_utils.o

all:
	#cd ../test && make
	make run

run:
	make illustrationlogic
	./illustrationlogic
	#./illustrationlogic ../test/input/15x15_01.txt
	#./illustrationlogic ../test/input/15x15_02.txt
	#./illustrationlogic ../test/input/30x30_01.txt
	#./illustrationlogic ../test/input/30x30_03.txt
	#./illustrationlogic ../test/input/50x60_01.txt
	#./illustrationlogic ../test/input/60x90_01.txt

illustrationlogic: $(objs)
	dmd -of=$@ $(objs)

%.obj: %.d
	dmd -unittest -c $<
	#dmd -c $<

#illustrationlogic: $(objs)
#	g++ -o $@ $(objs)

#%.o: %.cpp %.h
#	g++ -c -g -o $@ $<

clean:
	rm -f *.o
	rm -f *.obj
	rm -f *.exe

