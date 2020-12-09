LIBDIR := lib/

INCLUDEDIR :=
INCLUDEDIR += -Iinclude/
INCLUDEDIR += -I$(LIBDIR)include/

CC := gcc
CFLAGS := $(INCLUDEDIR) -pedantic -Wall -Wextra -Werror -Wno-misleading-indentation -Wno-unused-parameter

OBJS := hyper_client.o 

all: clean hyper
	@echo "Done!"

hyper: $(OBJS)
	$(CC) $< -o $@ -L$(LIBDIR) -lhyper 

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o hyper
