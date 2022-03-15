# This the root makefile and the function of this file is call other
# makefiles. Of course, you need first set the GCC compiler path/name, the
# simulator path/name and the board model in the respective directories:
#
# - src/Makefile
# - sim/Makefile
# - board/Makefile
#
# After configure each Makefile, you can just type 'make'

default: all

all:
	make -C src all
	make -C sim all
	make -C boards all

install:
	make -C boards install

clean:
	make -C src clean
	make -C sim clean
	make -C boards clean
