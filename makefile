all:
	g++ -c main.cpp
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive


main2:
	g++ -c main2.cpp
	g++ main2.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive

clean:
			rm -rf main.o
			rm -rf main2.o
