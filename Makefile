# Makefile for the smash program
CCC = g++
CXXFLAGS = -g -Wall 
CXXLINK = $(CCC)
OBJS = account.o atm.o bank.o main.o
RM = rm -f

all: Bank
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o Bank  $(OBJS)
# Creating the object files
account.o: account.cpp account.h includes.h
atm.o:  atm.cpp atm.h includes.h
bank.o: bank.cpp bank.h includes.h
main.o: main.cpp bank.h includes.h atm.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

