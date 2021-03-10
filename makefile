CC=g++

CFLAGS=-c

all: points

g: main_points.cpp func_file.cpp inter_contrl.cpp algorith.cpp neuron.cpp
	$(CC) main_points.cpp func_file.cpp inter_contrl.cpp algorith.cpp neuron.cpp -g -o debug

points: main_points.o func_file.o inter_contrl.o algorith.o neuron.o
	$(CC) main_points.o func_file.o inter_contrl.o algorith.o neuron.o -o points.exe

main_points.o: main_points.cpp
	$(CC) $(CFLAGS) main_points.cpp

func_file.o: func_file.cpp
	$(CC) $(CFLAGS) func_file.cpp

inter_contrl.o: inter_contrl.cpp
	$(CC) $(CFLAGS) inter_contrl.cpp

algorith.o: algorith.cpp
	$(CC) $(CFLAGS) algorith.cpp

neuron.o: neuron.cpp
	$(CC) $(CFLAGS) neuron.cpp