RM := rm -f
CC := gcc -std=c99
CFLAGS := -g
LDLIBS := -lm

SOURCES := $(wildcard *.c)
OBJECTS := $(SOURCES:%.c=%.o)
DEPS := $(SOURCES:%.c=%.d)
BINARY := maze62
FILES_TO_CLEAN := $(OBJECTS) $(DEPS)

.PHONY : all clean realclean

all : $(BINARY)

clean :
	$(RM) $(FILES_TO_CLEAN)

realclean : FILES_TO_CLEAN += $(BINARY)
realclean : clean

-include $(DEPS)

$(OBJECTS) : %.o : %.c
	$(CC) $(CFLAGS) -c -MMD -o $@ $<

$(BINARY) : $(OBJECTS)
	$(CC) -o $@ $^ $(LDLIBS)
