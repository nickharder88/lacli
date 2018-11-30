CFLAGS := -g -Wall
CC := cc -lm

OBJDIR := obj
SRCDIR := interpreter

SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))

debug : $(OBJFILES)
	$(CC) -O0 -o $@ $^

main : $(OBJFILES) test_main.o matrix.o dict.o
	$(CC) -o $@ $^

test_main.o: test_main.c
	$(CC) $(CFLAGS) -c -o test_main.o test_main.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c -o matrix.o matrix.c
	
dict.o: dict.c dict.h
	$(CC) $(CFLAGS) -c -o dict.o dict.c

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean :
	rm -rf main debug test_main.o matrix.o dict.o $(OBJDIR)
