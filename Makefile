# Makefile for the smash program
CXX = g++
CXXFLAGS = -std=c++11 -Wall
CCLINK = $(CXX)
OBJS = account.o atm.o bank.o main.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o smash $(OBJS)
# Creating the object files
account.o: account.cpp account.h
atm.o:  atm.cpp atm.h
bank.o: bank.cpp bank.h
main.o: main.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*

