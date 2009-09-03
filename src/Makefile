BIN = rotella
INSTALL_DIR = ~/bin
SRCS = \
    Url.cc \
    Conf.cc \
    Command.cc \
    utils.cc \
    main.cc
INCLUDES =
LIBS =



CC = g++
CFLAGS = -Wfatal-errors -Werror -Wall -Wextra -std=c++98 -pedantic-errors -g
LFLAGS =
OBJS = $(SRCS:.cc=.o)
DEPS = $(OBJS:.o=.d)
INSTALL_BIN=$(INSTALL_DIR)/$(BIN)



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
	valgrind --tool=memcheck --show-reachable=yes --leak-check=full -q ./$(BIN)

debugvalgrind: $(BIN)
	valgrind --tool=memcheck --show-reachable=yes --leak-check=full -q ./$(BIN) -d


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

.cpp.o:
	$(CC) -MMD $(CFLAGS) -c -o $@ $<
	@sed -i -e '1s,\($*\)\.o[ :]*,\1.o $*.d: ,' $*.d

-include $(DEPS)
