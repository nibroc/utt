CXXFLAGS+=-Wall -Werror -Wextra -Weffc++ \
	    -Wstrict-aliasing -pedantic -std=c++11
LDLIBS+=

CXX=clang++

MAIN_SRC=main.cpp
TEST_SRC=test.cpp
SRCS=

MAIN_EXE=utt
TEST_EXE=test
DEPS=.depend

ALL_SRCS=$(MAIN_SRC) $(TEST_SRC) $(SRCS)
MAIN_OBJ=$(patsubst %.cpp, %.o, $(MAIN_SRC))
TEST_OBJ=$(patsubst %.cpp, %.o, $(TEST_SRC))
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
ALL_OBJ=$(patsubst %.cpp, %.o, $(ALL_SRCS))

release: CXXFLAGS+=-O2
release: build

debug: CXXFLAGS+=-O0 -g -fno-omit-frame-pointer
debug: LDFLAGS+=-g
debug: build

test: CXXFLAGS+=-O0 -g -fno-omit-frame-pointer
test: LDFLAGS+=-g

build: depend $(MAIN_OBJ) $(OBJS)
	$(CXX) -o $(MAIN_EXE) $(MAIN_OBJ) $(OBJS) $(LDFLAGS) $(LDLIBS)

test: depend $(TEST_OBJ) $(OBJS)
	$(CXX) -o $(TEST_EXE) $(TEST_OBJ) $(OBJS) $(LDFLAGS) $(LDLIBS)

depend: $(ALL_SRCS)
	$(CXX) $(CXXFLAGS) -MM $(ALL_SRCS) > $(DEPS)

clean:
	$(RM) $(RMFLAGS) $(MAIN_EXE) $(TEST_EXE) $(ALL_OBJ) $(DEPS)

-include $(DEPS)
