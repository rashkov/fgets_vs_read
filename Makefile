BIN = main
SRCS= main.c
OBJS= main.o

PKGS =

CFLAGS  += -g -Wall -Werror
CPPFLAGS+= -I. -D_GNU_SOURCE
LDFLAGS += -lm

PREFIX ?= /usr

# No user serviceable parts below this line.
#PPFLAGS+= $(shell pkg-config --silence-errors --cflags $(PKGS))
#LDFLAGS += $(shell pkg-config --silence-errors --libs $(PKGS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) ${LDFLAGS} -o ${BIN}

$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $<

clean:
	-rm -rf $(BIN) *.o *.core

.PHONY: clean

-include $(DEPS)
