CC ?= gcc
CFLAGS ?= -Wall -Werror
ipath = $(realpath include)
lpath = $(realpath lib)
slib = $(realpath lib/mylib.a)
export

all: src

.PHONY: src
src: lib
	make -C src

.PHONY: lib
lib:
	make -C lib

install: src
	cp src/executable /home/demens/tmp/bin/
	cp lib/libmylib.so /home/demens/tmp/lib/

uninstall:
	rm -rf /home/demens/tmp/bin/executable
	rm -rf /home/demens/tmp/lib/libmylib.so

clean:
	make clean -C src
	make clean -C lib

