CC=g++

CFLAGS=-c

all: points interface

interface: main_interface.o Interface.o Configs.o my_functions.o
	$(CC) main_interface.o Interface.o Configs.o my_functions.o -o client.exe

main_interface.o: main_interface.cpp
	$(CC) $(CFLAGS) main_interface.cpp

g: main_server.cpp BinMatrix.cpp Buffer.cpp Cloud.cpp Cluster.cpp Configs.cpp \
	Controller.cpp Delaunay.cpp EMalgorithm.cpp Field.cpp \
	FindClusters.cpp Forel.cpp Hierarch.cpp Server.cpp \
	KerKMeans.cpp KMeans.cpp my_functions.cpp Point.cpp \
	Tree.cpp Triangle.cpp Triangulation.cpp WaveClusters.cpp
	$(CC) main_server.cpp BinMatrix.cpp Buffer.cpp Cloud.cpp Cluster.cpp Configs.cpp \
		Controller.cpp Delaunay.cpp EMalgorithm.cpp Field.cpp \
		FindClusters.cpp Forel.cpp Hierarch.cpp Server.cpp \
		KerKMeans.cpp KMeans.cpp my_functions.cpp Point.cpp \
		Tree.cpp Triangle.cpp Triangulation.cpp WaveClusters.cpp -g -o debug

points: main_server.o BinMatrix.o Buffer.o Cloud.o Cluster.o Configs.o \
	Controller.o Delaunay.o EMalgorithm.o Field.o \
	FindClusters.o Forel.o Hierarch.o Server.o \
	KerKMeans.o KMeans.o my_functions.o Point.o \
	Tree.o Triangle.o Triangulation.o WaveClusters.o
	$(CC) main_server.o BinMatrix.o Buffer.o Cloud.o Cluster.o Configs.o \
		Controller.o Delaunay.o EMalgorithm.o Field.o \
		FindClusters.o Forel.o Hierarch.o Server.o \
		KerKMeans.o KMeans.o my_functions.o Point.o \
		Tree.o Triangle.o Triangulation.o WaveClusters.o -o server.exe

main_server.o: main_server.cpp
	$(CC) $(CFLAGS) main_server.cpp

BinMatrix.o: BinMatrix.cpp
	$(CC) $(CFLAGS) BinMatrix.cpp

Buffer.o: Buffer.cpp
	$(CC) $(CFLAGS) Buffer.cpp

Cloud.o: Cloud.cpp
	$(CC) $(CFLAGS) Cloud.cpp

Cluster.o: Cluster.cpp
	$(CC) $(CFLAGS) Cluster.cpp

Configs.o: Configs.cpp
	$(CC) $(CFLAGS) Configs.cpp

Controller.o: Controller.cpp
	$(CC) $(CFLAGS) Controller.cpp

Delaunay.o: Delaunay.cpp
	$(CC) $(CFLAGS) Delaunay.cpp

EMalgorithm.o: EMalgorithm.cpp
	$(CC) $(CFLAGS) EMalgorithm.cpp

Field.o: Field.cpp
	$(CC) $(CFLAGS) Field.cpp

FindClusters.o: FindClusters.cpp
	$(CC) $(CFLAGS) FindClusters.cpp

Forel.o: Forel.cpp
	$(CC) $(CFLAGS) Forel.cpp

Hierarch.o: Hierarch.cpp
	$(CC) $(CFLAGS) Hierarch.cpp

Interface.o: Interface.cpp
	$(CC) $(CFLAGS) Interface.cpp

KerKMeans.o: KerKMeans.cpp
	$(CC) $(CFLAGS) KerKMeans.cpp

KMeans.o: KMeans.cpp
	$(CC) $(CFLAGS) KMeans.cpp

my_functions.o: my_functions.cpp
	$(CC) $(CFLAGS) my_functions.cpp

Point.o: Point.cpp
	$(CC) $(CFLAGS) Point.cpp

Tree.o: Tree.cpp
	$(CC) $(CFLAGS) Tree.cpp

Triangle.o: Triangle.cpp
	$(CC) $(CFLAGS) Triangle.cpp

Triangulation.o: Triangulation.cpp
	$(CC) $(CFLAGS) Triangulation.cpp

WaveClusters.o: WaveClusters.cpp
	$(CC) $(CFLAGS) WaveClusters.cpp

Server.o: Server.cpp
	$(CC) $(CFLAGS) Server.cpp
