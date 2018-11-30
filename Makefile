CFLAGS := -g -Wall
CC := cc

OBJDIR := obj
SRCDIR := src 

SRCFILES := $(foreach sdir, $(SRCDIR), $(wildcard $(sdir)/*.c))
OBJFILES := $(patsubst $(foreach sdir, $(SRCDIR), $(sdir)/%.c),$(OBJDIR)/%.o,$(SRCFILES))

main : $(OBJFILES)
	$(CC) -o $@ $^

$(OBJDIR)/%.o: $(foreach sdir, $(SRCDIR), $(sdir)/%.c)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean :
	rm -rf main $(OBJDIR)
