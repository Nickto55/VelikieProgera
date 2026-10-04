# Makefile для компиляции option_2.cpp с X11

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lX11
TARGET = option_2
SOURCE = option_2.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
