# Macros
CC = gcc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic -O -Wconversion -std=c89 -Wno-implicit-fallthrough

OBJECT = main.o dp_console.o Strength.o
EXE = Strength.exe
ERASE = rm

# Targets
$(EXE) : $(OBJECT)
	$(CC) -o $(EXE) $(OBJECT)
	$(ERASE) $(OBJECT)

%.o : %.c
	$(CC) -o $@ $(CFLAGS) -c $<

clean :
	$(ERASE) $(EXE)

run :
	./$(EXE)