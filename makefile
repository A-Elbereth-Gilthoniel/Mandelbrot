all:
	g++ -c main.cpp
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive


main2:
	g++ -c -march=znver3 main2.cpp -O3
	g++ main2.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive

clean:
			rm -rf main.o
			rm -rf main2.o
