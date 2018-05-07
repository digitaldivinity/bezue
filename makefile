bezue.o : bezue.cpp
	g++ -c bezue.cpp
bezue : bezue.o 
	g++ bezue.o -o bezue -lsfml-graphics -lsfml-window -lsfml-system
run: bezue
	./bezue
