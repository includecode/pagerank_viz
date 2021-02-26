#variable du compilateur gcc
CC = g++ -I ./eigen-3.3.9/

#The final ecacutable name
EXEC = app

#All files with .c extension
SRC = $(wildcard *.cpp)

#All files with .c are copied with .o extension
OBJ = $(SRC:.c=.o)

all : app
$(EXEC) : $(OBJ)
	$(CC) -o $@ $^
%.o : %.c
	$(CC) -o $@ -c $<
clean:
	rm -rf *.o
mrproper : clean
	rm -rf $(EXEC)