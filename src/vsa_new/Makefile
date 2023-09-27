CC=gcc
vlg = valgrind --leak-check=yes --track-origins=yes
G = gdb
GDFLAGS = -ansi -pedantic-errors -Wall -Wextra -g3 $(DEPFLAGS)
GCFLAGS = -ansi -pedantic-errors -Wall -Wextra -DNDEBUG -o3 $(DEPFLAGS)
DEPFLAGS = -MP -MD

DIR = .


TARGET = a.out
CFILES = $(foreach D,$(DIR),$(wildcard $(D)/*.c))
# search for files name stack.c in loop (foreach)
#stack.c stack_test.c
OBJECTS = $(patsubst %.c,%.o,$(CFILES))
#stack.o  stack_test.o - $(OBJECTS)
DEPFILES = $(patsubst %.c,%.d,$(CFILES))

#LN1 = ln -rs /home/yohai/yohai.azoulai/ds/src/zZz/zZz.h ~/yohai.azoulai/ds/include/zZz
#LN2 = ln -rs /home/yohai/yohai.azoulai/ds/src/zZz/zZz_test.c ~/yohai.azoulai/ds/test/zZz_test
#LN1R = ~/yohai.azoulai/ds/include/zZz
#LN2R = ~/yohai.azoulai/ds/test/zZz_test

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJECTS)
	$(CC) $(GDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(GDFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJECTS) $(DEPFILES)

purge:
	rm -rf $(OBJECTS) $(DEPFILES) $(TARGET) $(LN1R) $(LN2R)

%.so: %.o %.h
	$(CC) $(GDFLAGS) -fPIC -shared -o $@ $(TARGET).c -lc

lib: %.o
	ar rcs lib_arc.a %.o

release: $(OBJECTS)
	$(CC) $(GCFLAGS) $^ -o $(TARGET)

vlg : $(TARGET)
	$(vlg) ./$^

debug : $(TARGET)
	$(G) ./$(TARGET)

soft : $(TARGET)
	$(LN1)
	$(LN2)

diff:
	$(info The status of the repository, and the volume of per-liter changes:)
	@git status
	@git diff --stat

-include $(DEPFILES)
