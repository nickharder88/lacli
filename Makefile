CFLAGS := -g -Wall
CC := cc -g

OBJDIR := obj
SRCDIR := src 

SRCFILES := $(shell find . -name '*.c')
OBJFILES := $(patsubst $(shell find . -name '*.c')/%.c, $(OBJ)/%.o, $(SRCFILES))

main : $(OBJFILES)
	$(CC) $^ -o $@

$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean :
	rm -rf main $(OBJDIR)
