CXX = g++
CXXFLAGS = -O3 -std=c++20 -lfmt
SRC = $(wildcard src/*.cpp)
OUT = build

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
