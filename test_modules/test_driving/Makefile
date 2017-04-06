all: test_driving

clean: 
	rm build/*.o
	rm test_driving

test_driving: build/test_driving.o build/TdBike.o
	g++ build/test_driving.o build/TdBike.o -I/usr/local/include -L/usr/local/lib -lSDL2 -o test_driving

build/test_driving.o: src/test_driving.cpp
	g++ -std=gnu++11 -c src/test_driving.cpp -I/usr/local/include -L/usr/local/lib -lSDL2 -o build/test_driving.o

build/TdBike.o: src/TdBike.cpp src/TdBike.h
	g++ -std=gnu++11 -c src/TdBike.cpp -I/usr/local/include -L/usr/local/lib -lSDL2 -o build/TdBike.o
