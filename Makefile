OBJS	= build/main.obj build/logic.obj build/ltoa.obj build/malloc.obj build/stdfunc.obj
TEST_OBJS	= build/main_test.o build/logic.o build/ltoa.o build/malloc.o build/stdfunc.o
SOURCE	= src/main.c
HEADER	= 
OUT	= qr
CC	= g++ -std=c++98
WIN_CC	= wine ~/Downloads/tcc/tcc.exe
FLAGS	= -c -Wall
TEST_FLAGS	= -g -c -Wall -I ./include/
LFLAGS	= -luser32
TEST_LFLAGS	= -L ./lib/ -lgtest -lpthread -lgtest_main

all: $(OBJS)
	$(WIN_CC) $(OBJS) -o build/$(OUT).exe $(LFLAGS)

build/main.obj: src/main.c
	$(WIN_CC) $(FLAGS) $? -o $@

build/logic.obj: src/logic.c
	$(WIN_CC) $(FLAGS) $? -o $@

build/ltoa.obj: src/ltoa.c
	$(WIN_CC) $(FLAGS) $? -o $@

build/malloc.obj: src/malloc.c
	$(WIN_CC) $(FLAGS) $? -o $@

build/stdfunc.obj: src/stdfunc.c
	$(WIN_CC) $(FLAGS) $? -o $@

build/logic.o: src/logic.c
	$(CC) $(FLAGS) $? -o $@

build/ltoa.o: src/ltoa.c
	$(CC) $(FLAGS) $? -o $@

build/malloc.o: src/malloc.c
	$(CC) $(FLAGS) $? -o $@

build/stdfunc.o: src/stdfunc.c
	$(CC) $(FLAGS) $? -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) build/$(OUT).exe build/$(OUT)_test

run: build/$(OUT).exe
	wine build/$(OUT).exe

test: $(TEST_OBJS)
	$(CC) -g $(TEST_OBJS) -o build/$(OUT)_test $(TEST_LFLAGS) && ./build/$(OUT)_test && rm -f $(TEST_OBJS) build/$(OUT)_test

build/main_test.o: test/main_test.cpp
	g++ $(TEST_FLAGS) $? -o $@