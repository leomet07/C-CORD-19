OBJS	= cJSON.o main.o
SOURCE	= cJSON.c main.cpp
HEADER	= 
OUT	= Made.exe
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

cJSON.o: cJSON.c
	$(CC) $(FLAGS) cJSON.c 

main.o: main.cpp
	$(CC) $(FLAGS) main.cpp 


clean:
	rm -f $(OBJS) $(OUT)