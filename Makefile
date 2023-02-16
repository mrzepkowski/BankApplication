CODE1=BankApp
HFILE=BankSystem
BASE=base.txt

OBJS1=$(CODE1).o

EXEC1=$(CODE1).x

CC=g++
DIR = $(notdir $(CURDIR))

CFLAGS= -Wall -std=c++11

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

$(EXEC1): $(CODE1).cpp $(HFILE).h
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: all run clean tar

all: $(EXEC1)

run: $(EXEC1)
	./$(EXEC1) $(BASE)


clean:
	rm -f *.o  *~ *.a *.so *.x *.out; rm -rf

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )