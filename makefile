CC := $(CROSS_COMPILE)gcc

CFLAGS := -Wall -Ofast -fopenmp
LDFLAGS := -I/usr/include/freetype2 -Icommon

ifndef CROSS_COMPILE
	CFLAGS += -march=native -flto
	LDFLAGS += -march=native -flto
endif

LDLIBS := -ljpeg -lfreetype -lpng -lz -lc -lm

SRC := common/graphic.c common/touch.c common/image.c common/task.c gui.c board.c main.c

ifdef simulate
	CFLAGS+=-DUSING_SDL
	LDLIBS=-lSDL2
	SRC += sdl_wrapper.c
endif

OBJ := $(patsubst %.c, %.o, $(SRC))

connect4: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o connect4 $(OBJ) $(LDLIBS)

clean:
	rm -f connect4 $(OBJ)

%.o: %.c common/common.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c -o $@ $< $(LDLIBS)

