EXE = valley
SRC_DIR = src
OBJ_DIR = obj
SRC = $(wildcard $(SRC_DIR)/*.cpp )
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CPP_FLAGS += -Iinclude
CFLAGS += -Wall
#CXXFLAGS += -Iinclude

.PHONY: all clean

all: $(EXE)

$(EXE): obj $(OBJ)
	$(CXX) $(OBJ) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPP_FLAGS) $(CFLAGS) -c $< -o $@

obj:
	mkdir $@

clean:
	$(RM) $(OBJ) $(EXE)
