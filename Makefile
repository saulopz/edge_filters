CXX = clang++
CXXFLAGS = -g
LDFLAGS = -lSDL2 -lSDL2_image

SRC_DIR = src
BIN_DIR = .

EDGE_SRC = $(SRC_DIR)/edge.cpp $(SRC_DIR)/net.cpp
EDGE_BIN = $(BIN_DIR)/edge

TRAIN_SRC = $(SRC_DIR)/train.cpp
TRAIN_BIN = $(BIN_DIR)/train

all: $(EDGE_BIN) $(TRAIN_BIN)

$(EDGE_BIN): $(EDGE_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(TRAIN_BIN): $(TRAIN_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(EDGE_BIN) $(TRAIN_BIN)

.PHONY: all clean