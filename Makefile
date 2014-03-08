CXXFLAGS+=-Wall -Werror -Wextra -Weffc++ \
	    -Wstrict-aliasing -pedantic -std=c++11
LDLIBS+=

CXX=clang++

SRCS=main.cpp
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
EXE=utt
DEPS=.depend

release: CXXFLAGS+=-O2
release: build

debug: CXXFLAGS+=-O0 -g -fno-omit-frame-pointer
debug: LDFLAGS+=-g
debug: build

build: depend $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LDFLAGS) $(LDLIBS)

depend: $(SRCS)
	$(CXX) $(CXXFLAGS) -MM $(SRCS) > $(DEPS)

clean:
	$(RM) $(RMFLAGS) $(EXE) $(OBJS) $(DEPS)

-include $(DEPS)
