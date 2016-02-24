# Directory and exe configuration
BLDDIR	= ../build/client
OUTDIR	= ../bin
SRCDIR  = src
INCDIR  = include

INSDIR  = /usr/local/bin

CC = gcc
OUTNAME = raspboot
# End configuration

OUT = $(OUTDIR)/$(OUTNAME)
SRCS = $(wildcard $(SRCDIR)/*.c)
DEPS = $(wildcard $(INCDIR)/*.h)
CMNS = $(wildcard ../common/*.h)
OBJS = $(addprefix $(BLDDIR)/, $(notdir $(SRCS:.c=.o)))
CFLAGS = -Wall -Werror -O3 -I./include -I../

all: directories $(OUT)

$(BLDDIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(CMNS)
	$(CC) -c $(CFLAGS) -o $@ $<

$(OUT): $(OBJS)
	$(CC) -o $@ $^

.PHONY: clean directories install
.SUFFIXES:
.SECONDARY:

clean:
	rm -rf $(BLDDIR)
	rm -f $(OUT)

directories:
	mkdir -p $(BLDDIR)
	mkdir -p $(OUTDIR)

install: $(OUT)
	install -m 0755 $(OUT) $(INSDIR)/$(OUTNAME)