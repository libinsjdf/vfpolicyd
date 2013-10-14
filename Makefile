MAKE=	make

all:
		@echo "type 'make linux' for linux version"
		@echo "type 'make bsd' for BSD version"
linux:
		@${MAKE} -f Makefile.linux
bsd:
		@${MAKE} -f Makefile.bsd
clean:
		@echo "cleaning..."
		rm -rf *.o
		rm -rf vfpolicyd
