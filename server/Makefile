# Directory and exe configuration
BLDDIR	= ../build/server
OUTDIR	= ../bin
SRCDIR  = src
INCDIR  = include
ASMDIR  = asm

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
CP = arm-none-eabi-objcopy

OUTNAME = raspboot-server
# End configuration

OUT = $(OUTDIR)/$(OUTNAME)
SRCS = $(wildcard $(SRCDIR)/*.c)
ASMS = $(wildcard $(ASMDIR)/*.s)
DEPS = $(wildcard $(INCDIR)/*.h)
CMNS = $(wildcard ../common/*.h)
OBJS = $(addprefix $(BLDDIR)/, $(notdir $(SRCS:.c=.o)))
OBJS += $(addprefix $(BLDDIR)/, $(notdir $(ASMS:.s=.o)))
CFLAGS = -Wall -Werror -O3 -nostdlib -nostartfiles -ffreestanding -I./include -I../

all: directories $(OUT)

$(BLDDIR)/%.o: $(SRCDIR)/%.c $(DEPS) $(CMNS)
	$(CC) -c $(CFLAGS) -o $@ $<

$(BLDDIR)/%.o: $(ASMDIR)/%.s
	$(AS) $^ -o $@

$(BLDDIR)/%.elf: $(OBJS)
	$(LD) $^ -T memory.ld -o $@

$(OUT): $(BLDDIR)/$(OUTNAME).elf
	$(CP) $< -O binary $@

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