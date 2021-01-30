LIBDIR := lib/

INCLUDEDIR :=
INCLUDEDIR += -Iinclude/
INCLUDEDIR += -I$(LIBDIR)

CC := gcc
CFLAGS := $(INCLUDEDIR) -pedantic -Wall -Wextra -Werror -Wno-misleading-indentation -Wno-unused-parameter -Wno-unused-function

OBJS := hyper_client.o 

all: clean hyper
	@echo "Done!"

hyper: $(OBJS)
	$(CC) $< -o $@ 

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o hyper
