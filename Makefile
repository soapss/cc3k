CXX= g++
CXXFLAGS=-std=c++14 -g -MMD 
OBJECTS=main.o cc3k.o floor.o level.o defaultlevel.o stairway.o cell.o entity.o character.o \
		message.o color.o \
		player.o  drow.o goblin.o shade.o troll.o vampire.o \
		gold.o smallPile.o normalPile.o merchantHoard.o dragonHoard.o \
		item.o potion.o restoreHealth.o poisonHealth.o woundAtk.o boostAtk.o boostDef.o woundDef.o \
		subject.o textObserver.o graphicalObserver.o window.o \
		enemy.o orc.o dragon.o merchant.o elf.o halfling.o dwarf.o human.o \
		pathfinder.o dragonslayer.o

DEPENDS=${OBJECTS:.o=.d}
EXEC=cc3k


${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} -lX11 -lXpm
-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
