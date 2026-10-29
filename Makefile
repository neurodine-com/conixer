CXX = g++
INCLUDE_DIRS = $(CURDIR)/src
OUTPUT_DIR = bin
OBJ_DIR = objs
DEP_DIR = deps

SOURCES = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h)

OBJECTS = $(addprefix $(OBJ_DIR)/,$(notdir $(basename $(SOURCES))))
DEPS = $(addsuffix .d,$(patsubst %,$(OBJ_DIR)/%.o, $(SOURCES)))

LDFLAGS = -lpthread

UPGRADE_MY_STANDARDS = -std=c++20
USE_CHILLY_OLD = -std=c++14


CXXFLAGS_STANDARIZE_VER = $(UPGRADE_MY_STANDARDS)
# CXXFLAGS_STANDARIZE_VER = $(USE_CHILLY_OLD)
CXXFLAGS = -Wall $(CXXFLAGS_STANDARIZE_VER)

.PHONY: all clean run

all:	Comixer
	@echo "Building Comixer..."

Comixer:	src/main.cpp 
	@mkdir -p $(OUTPUT_DIR) $(DEP_DIR) $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIRS) $(SOURCES) \
         -o $(OUTPUT_DIR)/comixer \
         $(LDFLAGS)

clean:
	rm -rf $(OBJECTS) $(DEPS) *.o main.cpp objs deps objs/*.d bin/ 

run: all
	./bin/comixer

%.o: %.cpp %*.h
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIRS) -c $< -o $@
