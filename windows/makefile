all:
	make resolve
	./resolve < input/iraroji_15_15_-01.txt
	#./resolve < input/iraroji_15_15_-02.txt
	#./resolve < input/iraroji_30_40_-01.txt
	#./resolve < input/iraroji_40_35_-01.txt
	#./resolve < input/iraroji_50_60_-01.txt
	#./resolve < input/iraroji_60_50_-01.txt
	#./resolve < input/iraroji_60_50_-02.txt
	#./resolve < input/iraroji_60_50_-03.txt
	#./resolve < input/iraroji_60_50_-04.txt
	#./resolve < input/iraroji_60_50_-05.txt
	./resolve < input/iraroji_60_50_-06.txt
	#./resolve < input/iraroji_60_80_-01.txt
	#./resolve < input/iraroji_60_80_-02.txt
	#./resolve < input/iraroji_60_90_-01.txt
	#./resolve < input/iraroji_60_90_-02.txt

IllustrationLogic.exe: IllustrationLogic.cpp
	dmc -Aer -L/SU:windows gdi32.lib comdlg32.lib winmm.lib IllustrationLogic.cpp

clean:
	rm IllustrationLogic.exe
	rm IllustrationLogic.map
	rm IllustrationLogic.obj

resolve: resolve.cpp IllustrationLogicSolver.cpp
	g++ -std=c++14 -o resolve resolve.cpp IllustrationLogicSolver.cpp


