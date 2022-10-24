CC=g++
CFLAGS= -Wall -g
OBJDIR=obj
INC=-Iinclude

OBJS= $(OBJDIR)/lsfit.o $(OBJDIR)/seg_reg.o $(OBJDIR)/read_data.o $(OBJDIR)/2d_utils.o

main: main.cpp $(OBJS)
	$(CC) $(INC) -o main main.cpp $(OBJS)

$(OBJDIR)/2d_utils.o : src/2d_utils.cpp
	$(CC) $(CFLAGS) $(INC) -c src/2d_utils.cpp -o $(OBJDIR)/2d_utils.o

$(OBJDIR)/seg_reg.o: src/seg_reg.cpp
	$(CC) $(CFLAGS) $(INC) -c src/seg_reg.cpp -o $(OBJDIR)/seg_reg.o

$(OBJDIR)/lsfit.o: src/lsfit.cpp
	$(CC) $(CFLAGS) $(INC) -c src/lsfit.cpp -o $(OBJDIR)/lsfit.o

$(OBJDIR)/read_data.o : src/read_data.cpp
	$(CC) $(CFLAGS) $(INC) -c src/read_data.cpp -o $(OBJDIR)/read_data.o

clean: 
	rm $(OBJDIR)/*.o main
