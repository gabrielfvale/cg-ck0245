CC=g++
CFLAGS=-I -Wall
DEPS= Point.h Vector3.h Ray.h Plane.h Sphere.h Cylinder.h Cone.h AABB.h Cube.h
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
cgmake: Point.cpp Vector3.cpp Ray.cpp Plane.cpp Sphere.cpp Cylinder.cpp Cone.cpp AABB.cpp OBB.cpp main.cpp
	$(CC) -o program.out Point.cpp Vector3.cpp Ray.cpp Plane.cpp Sphere.cpp Cylinder.cpp Cone.cpp AABB.cpp OBB.cpp main.cpp -I.
