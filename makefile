clusterer: clusterer.o
	g++ -o clusterer clusterer.o -std=c++11

clusterer.o: clusterer.cpp
	g++ -c -o clusterer.o clusterer.cpp -std=c++11


run: clusterer
	./clusterer $(args)

clean:
	rm -rf clusterer.o clusterer