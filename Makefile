TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -nf ./hello.db
	./$(TARGET) -f ./hello.db -a "Jimmy, StuckStrasse, 120"
default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -g -o  $@ $?

obj/%.o : src/%.c
	gcc -g -c  $< -o $@ -Iinclude


