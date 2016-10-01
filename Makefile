EXECUTABLE = swpathtracer

CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -fopenmp

ifeq ($(DEBUG), 1)
CXXFLAGS += -DDEBUG -O0 -g3
else
CXXFLAGS += -DNDEBUG -O2 -g0
endif

CPPFLAGS += -MMD \
			-I/home/christian/work/common/glm-0.9.7.0 \
			-I/usr/include/lua5.2

LDFLAGS = -L/usr/lib/gcc/x86_64-linux-gnu/4.8 \
		  -L/usr/lib/x86_64-linux-gnu \
		  -L/usr/lib 

LDLIBS = -lgomp -lassimp -llua5.2

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)

.cpp.o:

-include $(OBJECTS:.o=.d)

.PHONY: clean distclean

clean:
	rm *.o *.d

distclean: clean
	rm $(EXECUTABLE)

