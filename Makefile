CC := g++
CFLAGS := -O -Wall -g
TARGET := program.out
SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(patsubst src/%.cpp, obj/%.o, $(SRC_FILES))

all: $(TARGET)

fresh: clean $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) -o $@ $^
obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(TARGET) obj/*.o
