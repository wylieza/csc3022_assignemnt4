clusterer: clusterer.o ppm.o processor.o
	g++ -o clusterer clusterer.o ppm.o processor.o -std=c++11

clusterer.o: clusterer.cpp
	g++ -c -o clusterer.o clusterer.cpp -std=c++11

ppm.o: ppm.cpp ppm.h
	g++ -c -o ppm.o ppm.cpp -std=c++11

processor.o: processor.cpp processor.h ppm.h
	g++ -c -o processor.o processor.cpp -std=c++11


run: clusterer
	./clusterer $(args)

clean:
	rm -rf clusterer.o clusterer ppm.o processor.o

debug:
	g++ -g -o clusterer clusterer.o ppm.o processor.o -std=c++11
	gdb --args clusterer Gradient_Numbers_PPMS

test: clusterer
	./clusterer Gradient_Numbers_PPMS

testf: clusterer
	./clusterer Gradient_Numbers_PPMS -o results_output.txt

color: clusterer
	./clusterer Gradient_Numbers_PPMS -color