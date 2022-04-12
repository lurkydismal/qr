OBJS	= build/main.o build/ltoa.o build/malloc.o build/printChar.o build/stdfunc.o
TEST_OBJS	= build/main_test.o
SOURCE	= src/main.cpp
HEADER	= 
OUT	= qr.exe
CC	= g++
WIN_CC	= i686-w64-mingw32-g++-win32
FLAGS	= -g -c -Wall
TEST_FLAGS	= -g -c -Wall -I ./include/
LFLAGS	= lib/* -static-libstdc++ -static-libgcc
TEST_LFLAGS	= -L ./lib/ -lgtest -lpthread -lgtest_main

all: $(OBJS)
	$(WIN_CC) -g $(OBJS) -o build/$(OUT) $(LFLAGS)

build/main.o: src/main.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/ltoa.o: src/ltoa.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/malloc.o: src/malloc.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/printChar.o: src/printChar.asm
	wine ~/Downloads/nasm/nasm.exe -f win32 $? -o $@

build/stdfunc.o: src/stdfunc.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) build/$(OUT) build/$(OUT)_test

run: build/$(OUT)
	wine build/$(OUT)

test: $(TEST_OBJS)
	$(CC) -g $(TEST_OBJS) -o build/$(OUT)_test $(TEST_LFLAGS)
	./build/$(OUT)_test
	rm -f $(TEST_OBJS) build/$(OUT)_test

build/main_test.o: test/main_test.cpp
	$(CC) $(TEST_FLAGS) $? -o $@