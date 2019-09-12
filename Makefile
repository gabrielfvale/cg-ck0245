CC := g++
CFLAGS := -std=c++11 -O -Wall -g
TARGET := program.out
SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(patsubst src/%.cpp, obj/%.o, $(SRC_FILES))

all: checkdir $(TARGET)

fresh: clean checkdir $(TARGET)

checkdir: obj/

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^ -lSDL2

obj/:
	mkdir -p $@

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) obj/*.o
