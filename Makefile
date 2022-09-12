CC=g++
CFLAGS= -Wall -g
OBJS= lsfit.o seg_reg.o read_data.o

main: main.cpp $(OBJS)
	$(CC) -o main main.cpp $(OBJS)

seg_reg.o: seg_reg.hpp seg_reg.cpp lsfit.hpp constants.hpp
	$(CC) $(CFLAGS) -c seg_reg.cpp

lsfit.o: lsfit.cpp lsfit.hpp constants.hpp
	$(CC) $(CFLAGS) -c lsfit.cpp

read_data.o : read_data.cpp read_data.hpp 
	$(CC) $(CFLAGS) -c read_data.cpp

clean: 
	rm *.o main
