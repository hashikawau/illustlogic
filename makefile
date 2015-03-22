
TARGET=illustlogic.exe

all:
	g++ --std=c++11 -o${TARGET} src/main.cpp

clean:
	rm -f ${TARGET}

