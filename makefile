#MAKEFILE FOR PROJECT 'gol'
PROG = run
CC = g++
INC = -Iinc
OUT = lib/lib.a
LDFLAGS = -lm -pg
ODIR = obj
SDIR = src

_OBJS = main.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) -c $(INC) -o $@ $<
$(PROG): $(OBJS)
	$(CC) -o $@ $< $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o $(OUT) $(PROG) ./*.out