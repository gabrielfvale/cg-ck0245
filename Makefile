CC=g++
CFLAGS=-I -Wall
DEPS= Point.h Vector3.h Ray.h
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
cgmake: Point.cpp Vector3.cpp Ray.cpp main.cpp
	$(CC) -o program.out Point.cpp Vector3.cpp Ray.cpp main.cpp -I.
