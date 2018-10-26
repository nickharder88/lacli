CFLAGS := -g -Wall
CC := cc

OBJDIR := obj
SRCDIR := src

SRCFILES := $(wildcard $(SRCDIR)/*.c)
OBJFILES := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRCFILES))

debug : $(OBJFILES)
	$(CC) -O0 -o $@ $^

main : $(OBJFILES)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -rf main $(OBJDIR)
