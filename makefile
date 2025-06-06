CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

OBJS = main.o Assembler.o Reader.o Writer.o
DEPS = Assembler.h Reader.h Writer.h AxeUtil.h

TARGET = axe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)
