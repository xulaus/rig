SRC = $(notdir $(wildcard src/*.cpp))
OBJ = $(SRC:%.cpp=obj/release/%.o)
TEST_SRC = $(notdir $(wildcard tests/*.cpp))
TEST_OBJ = $(TEST_SRC:%.cpp=obj/testing/%.o)

CXXFLAGS := -std=c++11 -Wall -Wextra -Wshadow -Weffc++ -pedantic -O3 -g

SYS := $(shell gcc -dumpmachine)
ifneq (, $(findstring linux, $(SYS)))
	EXE := rig
	LIB_DIRS = /usr/local/lib
	INC_DIRS = /usr/local/include
	RMFLAGS := -R
endif

ifneq (, $(findstring mingw, $(SYS)))
	EXE := rig.exe
	RMFLAGS := /F
	RM := del
endif

ifneq (, $(findstring cygwin, $(SYS)))
	EXE := rig.exe
	RMFLAGS = -R
	LIB_DIRS = /usr/local/lib
endif

ifneq (, $(findstring apple, $(SYS)))
	EXE := rig
	RMFLAGS = -R
	LIB_DIRS = /opt/local/lib
	INC_DIRS = /opt/local/include
endif

override INC_DIRS += ./include

CXXFLAGS +=  $(INC_DIRS:%=-I%)
LDFLAGS  :=  $(CXXFLAGS) $(LIB_DIRS:%=-L%)

.PHONY : run all clean debug compile-only test    # .PHONY ignores files named all

all: $(EXE)

obj:
	@mkdir obj
	@mkdir "obj/release"
	@mkdir "obj/testing"

obj/rig.o:: rig.cpp
	@echo "Compiling object file $< ($@)..."
	@$(CXX) $(CXXFLAGS) rig.cpp -c -o obj/rig.o

obj/release/%.o:: src/%.cpp
	@echo "Compiling object file $< ($@)..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

obj/testing/%.o:: tests/%.cpp
	@echo "Compiling testing file $< ($@)..."
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ): | obj

compile-only: $(OBJ)
	@echo "Compiled All"

$(EXE): $(OBJ) obj/rig.o
	@echo "Linking object files..."
	@$(CXX) $(LDFLAGS) -o $@ obj/rig.o $(OBJ) $(LIBS)

run: $(EXE)
	@$(EXE)

test: $(OBJ) $(TEST_OBJ)
	@echo "Making test executable"
	@$(CXX) $(LDFLAGS) -o rig_test $(TEST_OBJ) $(OBJ) $(LIBS)

debug:
	@echo "$(LDFLAGS)"

clean:
	@$(RM) $(RMFLAGS) obj
	@$(RM) $(EXE)
