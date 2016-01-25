# Directory and exe configuration
BLDDIR = build
OUTDIR = bin
# End configuration

all: directories client server
.PHONY: directories clean client install server

client:
	$(MAKE) -C client

server:
	$(MAKE) -C server

install: all
	$(MAKE) -C client install
	
clean:
	rm -rf $(BLDDIR)
	rm -rf $(OUTDIR)

directories:
	mkdir -p $(BLDDIR)
	mkdir -p $(OUTDIR)