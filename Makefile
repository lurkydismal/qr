OBJS	= build/main.obj build/logic.obj build/ltoa.obj build/malloc.obj build/stdfunc.obj
TEST_OBJS	= build/main_test.o build/logic.o build/ltoa.o build/malloc.o build/stdfunc.o
SOURCE	= src/main.cpp
HEADER	= 
OUT	= qr
CC	= g++
WIN_CC	= i686-w64-mingw32-g++-win32
FLAGS	= -g -c -Wall -masm=intel
TEST_FLAGS	= -g -c -Wall -I ./include/
LFLAGS	= lib/* -static-libstdc++ -static-libgcc
TEST_LFLAGS	= -L ./lib/ -lgtest -lpthread -lgtest_main

all: $(OBJS)
	$(WIN_CC) -g $(OBJS) -o build/$(OUT).exe $(LFLAGS)

build/main.obj: src/main.cpp
	$(WIN_CC) $(FLAGS) -DEMPTY_PLAYER_INVENTORY $? -o $@

build/logic.obj: src/logic.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/ltoa.obj: src/ltoa.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/malloc.obj: src/malloc.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/printChar.obj: src/printChar.asm
	wine ~/Downloads/nasm/nasm.exe -f win32 $? -o $@

build/stdfunc.obj: src/stdfunc.cpp
	$(WIN_CC) $(FLAGS) $? -o $@

build/logic.o: src/logic.cpp
	$(CC) $(FLAGS) $? -o $@

build/ltoa.o: src/ltoa.cpp
	$(CC) $(FLAGS) $? -o $@

build/malloc.o: src/malloc.cpp
	$(CC) $(FLAGS) $? -o $@

build/printChar.o: src/printChar.asm
	wine ~/Downloads/nasm/nasm.exe -f elf32 $? -o $@

build/stdfunc.o: src/stdfunc.cpp
	$(CC) $(FLAGS) $? -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) build/$(OUT).exe build/$(OUT)_test

run: build/$(OUT).exe
	wine build/$(OUT).exe

test: $(TEST_OBJS)
	$(CC) -g $(TEST_OBJS) -o build/$(OUT)_test $(TEST_LFLAGS) && ./build/$(OUT)_test && rm -f $(TEST_OBJS) build/$(OUT)_test

build/main_test.o: test/main_test.cpp
	$(CC) $(TEST_FLAGS) $? -o $@