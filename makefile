# makefile for assignment 5
# Yibo Yang, 04/26/2016

EXE = vm
INCLUDES = TLB.h PageTable.h Memory.h Line.h FeistelNetwork.h 
OBJ = buddy.o BuddyAllocator.o LinkedList.o
OBJS = TLB.cpp PageTable.cpp BackingStore.cpp Memory.cpp Line.cpp main.cpp FeistelNetwork.cpp
CPP=g++

all: dir orig startgap feistel buddy

orig:	$(OBJS) $(INCLUDES)
	$(CPP) $(FLAGS) -o bin/$(EXE) $(OBJS)

startgap: $(OBJS) $(INCLUDES)
	$(CPP) -DSTART_GAP $(FLAGS) -o bin/$@ $(OBJS)

feistel: $(OBJS) $(INCLUDES)
	$(CPP) -DFEISTEL $(FLAGS) -o bin/$@ $(OBJS)

buddy: $(OBJ)
	$(CPP) -o bin/buddy $(OBJ)

testbuddy: $(OBJ)
	./bin/buddy 2048

test:
	./bin/vm -L memory_info.log a.out

clean:
	rm -f $(OBJ)
	rm -r ./bin/

dir:
	mkdir -p bin
