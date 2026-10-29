
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = gronsfeld_cipher
OBJS = main.o modAlphaCipher.o
all: $(TARGET)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
main.o: main.cpp modAlphaCipher.h
	$(CXX) $(CXXFLAGS) -c main.cpp
modAlphaCipher.o: modAlphaCipher.cpp modAlphaCipher.h
	$(CXX) $(CXXFLAGS) -c modAlphaCipher.cpp
clean:
	rm -f $(OBJS) $(TARGET)
