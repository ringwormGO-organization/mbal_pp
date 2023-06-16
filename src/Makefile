CC = g++
CFLAGS = -Wall -Wextra -pedantic -g --std=c++20 -lm

OBJ = obj

OBJDIR = $(OBJ)/objects
OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))
OBJS += $(addprefix $(OBJDIR)/, $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp)))

BINFOLDER = $(OBJ)/executable
BIN = $(BINFOLDER)/mbal++

.PHONY: all
all : $(OBJ) $(OBJDIR) $(BINFOLDER) $(BIN)

$(OBJ):
	mkdir $(OBJ)

$(OBJDIR):
	mkdir $(OBJDIR)

$(BINFOLDER):
	mkdir $(BINFOLDER)

$(BIN) : $(OBJS)
	$(CC) $(OBJS) -o $(BIN) $(CFLAGS)

$(OBJDIR)/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: production
production: CFLAGS := $(filter-out -g, $(CFLAGS))
production: | all
	@mv $(BINFOLDER)/mbal++ ../

.PHONY: run
run:
	@./$(BIN)

.PHONY: clean
clean:
	rm -rf $(wildcard *.o) objects

.PHONY: full_clean
full_clean:
	rm -rf $(OBJ)
