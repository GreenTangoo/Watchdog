CC=g++
CDFLAGS=-c -g -Wall -std=c++17
CRFLAGS=-c -std=c++17

ODIR=build-WideProtectSIEM

UTILITY_SOURCE := $(wildcard utility_module/*.cpp)
UTILITY_OBJ = $(addprefix $(ODIR)/, $(UTILITY_SOURCE:.cpp=.o))

DESCRIPTION_SOURCE := $(wildcard description_module/*.cpp)
DESCRIPTION_OBJ = $(addprefix $(ODIR)/, $(DESCRIPTION_SOURCE:.cpp=.o))

CORRELATION_SOURCE := $(wildcard correlation_module/*.cpp)
CORRELATION_OBJ = $(addprefix $(ODIR)/, $(CORRELATION_SOURCE:.cpp=.o))

AGGREGATION_SOURCE := $(wildcard aggregation_module/*.cpp)
AGGREGATION_OBJ = $(addprefix $(ODIR)/, $(AGGREGATION_SOURCE:.cpp=.o))

all: creation $(UTILITY_OBJ) $(DESCRIPTION_OBJ) $(CORRELATION_OBJ) $(AGGREGATION_OBJ)
	$(CC) $(CDFLAGS) main.cpp -o $(ODIR)/main.o
	$(CC) -g $(UTILITY_OBJ) $(DESCRIPTION_OBJ) \
		$(CORRELATION_OBJ) $(AGGREGATION_OBJ) \
		$(ODIR)/main.o -o $(ODIR)/main

$(ODIR)/%.o: %.cpp
	$(CC) $(CDFLAGS) $< -o $@

creation:
	@mkdir $(ODIR)
	@mkdir $(ODIR)/utility_module
	@mkdir $(ODIR)/description_module
	@mkdir $(ODIR)/correlation_module
	@mkdir $(ODIR)/aggregation_module

.PHONY: clean

clean:
	rm -rf $(ODIR)

