clusterer: clusterer.o ppm_image.o
	g++ -o clusterer clusterer.o ppm_image.o -std=c++11

clusterer.o: clusterer.cpp
	g++ -c -o clusterer.o clusterer.cpp -std=c++11

ppm_image.o: ppm_image.cpp ppm_image.h
	g++ -c -o ppm_image.o ppm_image.cpp


run: clusterer
	./clusterer $(args)

clean:
	rm -rf clusterer.o clusterer