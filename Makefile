CC=g++
CFLAGS=-I -Wall
DEPS= Point.h Vector3.h Ray.h Plane.h Sphere.h Cylinder.h Cone.h
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
cgmake: Point.cpp Vector3.cpp Ray.cpp Plane.cpp Sphere.cpp Cylinder.cpp Cone.cpp main.cpp
	$(CC) -o program.out Point.cpp Vector3.cpp Ray.cpp Plane.cpp Sphere.cpp Cylinder.cpp Cone.cpp main.cpp -I.
