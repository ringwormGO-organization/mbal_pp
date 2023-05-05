CC = g++
CFLAGS = -Wall -Wextra -pedantic -g --std=c++20

OBJ = obj

OBJDIR = $(OBJ)/objects
OBJS = $(addprefix $(OBJDIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))
OBJS += $(addprefix $(OBJDIR)/, $(patsubst src/%.cpp, %.o, $(wildcard src/*.cpp)))

BINFOLDER = $(OBJ)/executable
BIN = $(BINFOLDER)/mbal++

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

run:
	@./$(BIN)

clean:
	rm -rf $(wildcard *.o) objects

full_clean:
	rm -rf $(OBJ)
