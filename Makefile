# Directory and exe configuration
BLDDIR = build
OUTDIR = bin
DOCDIR = doc
# End configuration

all: directories client server
.PHONY: directories clean clean_doc client doc install server

client:
	$(MAKE) -C client

server:
	$(MAKE) -C server

install: all
	$(MAKE) -C client install

doc: 
	doxygen doxygen.cfg

clean_doc:
	rm -rf $(DOCDIR)

clean:
	rm -rf $(BLDDIR)
	rm -rf $(OUTDIR)
	rm -rf $(DOCDIR)

directories:
	mkdir -p $(BLDDIR)
	mkdir -p $(OUTDIR)