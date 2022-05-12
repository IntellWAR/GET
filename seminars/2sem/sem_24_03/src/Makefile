obj = 1.o main.o

flags = $(slib)
ifeq ($(LIBTYPE), DYNAMIC)
	flags = -lmylib -L$(lpath)
endif

all: build

%.o: %.c
	$(CC) $(CFLAGS) -I$(ipath) $< -c -o $@

build: $(obj)
	$(CC) $(obj) $(flags) $(CFLAGS) -o executable

clean:
	rm -rf ./*.o executable
