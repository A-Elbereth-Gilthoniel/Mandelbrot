all:
	g++ -c main.cpp -O3
	g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive


main2:
	g++ -c -march=znver3 main2.cpp -O3
	g++ main2.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive

main3:
	g++ -c -march=znver3 main3.cpp -O3
	g++ main3.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive

main4:
	g++ -c -march=znver3 main4.cpp -O3
	g++ main4.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system -fpermissive

clean:
			rm -rf main.o
			rm -rf main2.o
			rm -rf main3.o
			rm -rf main4.o
			rm -rf sfml-app
