FILESDIR=src

CODE1=$(FILESDIR)/BankApp
HFILE1=$(FILESDIR)/BankSystem
HFILE2=$(FILESDIR)/BankAccount
BASE=$(FILESDIR)/base.txt

OBJS1=$(CODE1).o

EXEC1=$(CODE1).x

CC=g++
DIR = $(notdir $(CURDIR))

CFLAGS= -Wall -std=c++17

%.o : %.cpp
	$(CC) $(CFLAGS) -c $<

$(EXEC1): $(CODE1).cpp $(HFILE1).o $(HFILE1).h $(HFILE2).h
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: all run clean tar

all: $(EXEC1)

run: $(EXEC1)
	./$(EXEC1) $(BASE)


clean:
	cd src; rm -f *.o  *~ *.a *.so *.x *.out; rm -rf

tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )