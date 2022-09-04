CC=g++
CFLAGS= -Wall -g
OBJS= point.o lsfit.o seg_reg.o

clean: rm *.o

main: main.cpp $(OBJS)
	$(CC) -o main main.cpp $(OBJS)

seg_reg.o: seg_reg.cpp point.hpp lsfit.hpp slice.hpp
	$(CC) $(CFLAGS) -c seg_reg.cpp

point.o: point.cpp point.hpp
	$(CC) $(CFLAGS) -c point.cpp

lsfit.o: lsfit.cpp lsfit.hpp
	$(CC) $(CFLAGS) -c lsfit.cpp

