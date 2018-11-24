SDIR =src
IDIR =include
CXX = gcc
CXXFLAGS = -g -Wall -I$(IDIR) -lm

ODIR=build

_DEPS= ss_helper.h ss.h
DEPS= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o ss.o ss_helper.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: bin/ss

$(OBJ): $(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	mkdir -p build
	$(CXX) -c -o $@ $< $(CXXFLAGS)

.PHONY: clean

clean:
	rm -f *~ core $(IDIR)/*~ -r $(ODIR)/ bin/

bin/ss: $(OBJ)
	mkdir -p bin
	$(CXX) -o $@ $^ $(CXXFLAGS)
