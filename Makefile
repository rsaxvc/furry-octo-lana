#debian appears to be making the switch to better multiarch support, but until pkgconfig handles it, hardcode away
LFLAGS  = -lSDL -lGL -lGLU
CFLAGS  = -O2 -g -Wall -Wextra
CPPFLAGS=$(CFLAGS)
SOURCES=`ls *.cpp`
BUILDS=missile

all: .depend missile

.depend:
	fastdep $(SOURCES) > .depend

-include .depend

missile:\
	draw_manager.o\
	explosion.o\
	game_state.o\
	timer.o\
	draw.o\
	grid.o\
	bullet.o\
	missile.o\
	entity.o\
	main.o\
	time.o\
	periodic_controller.o
	g++ -o $@ $^ ${LFLAGS}

clean:
	rm -f *.o .depend $(BUILDS)
