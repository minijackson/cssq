CXX            =  g++
CFLAGS         += -Wall -Wextra -std=c++11
CFLAGS         += -Wsuggest-attribute=pure -Wsuggest-attribute=const
CFLAGS         += -Ilib/hcxselect/src
LDFLAGS        += -Llib/hcxselect/src -lhcxselect -lstdc++ $(shell pkg-config --libs htmlcxx)

EXECS          = cssq
SOURCES        = $(wildcard src/*.cpp)
HEADERS        = $(wildcard src/*.hpp)
OBJECTS        = $(SOURCES:%.cpp=%.o)

PROFILE_CFLAGS = -Og -pg
DEBUG_CFLAGS   = -Og -g
RELEASE_CFLAGS = -O2

.PHONY: debug profile release clean all

release: CFLAGS += $(RELEASE_CFLAGS)
release: all

debug: CFLAGS += $(DEBUG_CFLAGS)
debug: all

profile: CFLAGS += $(PROFILE_CFLAGS)
profile: all

lib/hcxselect/src/libhcxselect.a:
	$(MAKE) -wC lib/hcxselect/src

clean:
	$(RM) $(EXECS) $(OBJECTS)

all: $(EXECS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $^ -o $@

cssq: $(OBJECTS) $(HEADERS) lib/hcxselect/src/libhcxselect.a
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
