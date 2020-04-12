clusterer: clusterer.o ppm.o
	g++ -o clusterer clusterer.o ppm.o -std=c++11

clusterer.o: clusterer.cpp
	g++ -c -o clusterer.o clusterer.cpp -std=c++11

ppm.o: ppm.cpp ppm.h
	g++ -c -o ppm.o ppm.cpp


run: clusterer
	./clusterer $(args)

clean:
	rm -rf clusterer.o clusterer

test: clusterer
	./clusterer "Gradient_Numbers_PPMS"