SRC := $(wildcard *.c)
OBJ := $(patsubst %.c,%.o,$(SRC))
CC := g++
CFLAGS := -O3
GSL := $$MY_GSL_DIR

circlea: circlea.o
	@$(CC) -I$(GSL)/include $(CFLAGS) -lgsl -lgslcblas -o $@ $< -L$(GSL)/lib
circlea.o: circlea.cpp
	@$(CC) -I$(GSL)/include $(CFLAGS) -lgsl -lgslcblas -c $< -L$(GSL)/lib
clean: neat
	@$(RM) circlea
	@$(RM) circlea.o
neat:
	@$(RM) *~ .*~
