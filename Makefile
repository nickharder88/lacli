CFLAGS := -Wall
CC := cc -g -lm

OBJDIR := obj/cli
SRCDIR := src/cli

SRCFILES := $(shell find src/ -name '*.c')
OBJFILES := $(patsubst $(shell find src/ -name '*.c')/%.c, $(OBJ)/%.o, $(SRCFILES))

prof : $(OBJFILES)
	$(CC) $^ -o $@ -pg

main : $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJFILES): | $(OBJDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean :
	rm -rf main prof gmon.out $(OBJDIR)
