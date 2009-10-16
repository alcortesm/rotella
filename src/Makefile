BIN = rotella
INSTALL_DIR = ~/bin
SRCS = \
    prompt.cc \
    http.cc \
    Url_test.cc \
    Url.cc \
    Conf.cc \
    Command.cc \
    utils.cc \
    main.cc
INCLUDES =
LIBS =



CC = g++
CFLAGS = -g3 -O0 -Wfatal-errors -Werror -Wall -Wextra -std=c++98 -pedantic-errors
LFLAGS = -g3 -lpthread
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)
INSTALL_BIN=$(INSTALL_DIR)/$(BIN)
VALGRIND_FLAGS = --tool=memcheck --show-reachable=yes --leak-check=full -q



.PHONY: all install clean nuke

all: $(BIN)

install: $(BIN)
	cp $(BIN) $(INSTALL_BIN)

uninstall:
	- rm -f $(INSTALL_BIN)

dist:
	tar -cvzf rotella.tgz $(SRCS)

TAGS:
	cscope -b -q *.cpp *.h

test:

valgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN)

debugvalgrind: $(BIN)
	valgrind $(VALGRIND_FLAGS) ./$(BIN) -d


clean:
	- rm -f $(OBJS)
	- rm -f $(BIN)
	- rm -f $(DEPS)
	- rm -f cscope.in.out cscope.out cscope.po.out
	- rm -f *~

nuke: clean
	- rm -f $(INSTALL_DIR)/$(BIN)


$(BIN): $(OBJS)
	$(CC) $(LFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LIBS)

.cc.o:
	$(CC) -MMD $(CFLAGS) -c -o $@ $<
	@sed -i -e '1s,\($*\)\.o[ :]*,\1.o $*.d: ,' $*.d

-include $(DEPS)
