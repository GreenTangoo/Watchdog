CC=g++
CDFLAGS=-c -g -Wall
CRFLAGS=-c

ODIR=build-WideProtectSIEM

UTILITY_SOURCES := $(wildcard utility_module/*.cpp)
UTILITY_OBJS = $(addprefix $(ODIR)/, $(UTILITY_SOURCES:.cpp=.o))

all: creation $(UTILITY_OBJS)
	$(CC) $(CDFLAGS) main.cpp -o $(ODIR)/main.o

$(ODIR)/%.o: %.cpp
	$(CC) $(CDFLAGS) $< -o $@

creation:
	@mkdir $(ODIR)
	@mkdir $(ODIR)/utility_module

