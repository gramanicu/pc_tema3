# Copyright 2018 Grama Nicolae

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -O3 -Wno-unused-result
LFLAGS = -lm
EXE = snowfight
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

# arhiva checker
CHECKER = https://ocw.cs.pub.ro/courses/_media/programare/teme_2018/check_snowfight.zip
CARCHIVE = check_snowfight.zip

# arhiva cu tema
ANAME = 312CA_GramaNicolae_Tema3.zip
ACONTENTS = README Makefile *.c *.h
AFLAGS = -FSr 

# compilarea programului
build: $(OBJ)
	$(CC) -o $(EXE) $^ $(LFLAGS) $(CFLAGS)
	rm -f *.o

.PHONY:build
# ruleaza programul
run: build
		./$(EXE)
.PHONY:run

#arhiveaza tema
pack:build prepare 
	zip $(AFLAGS) $(ANAME) $(ACONTENTS) 
	rm README
.PHONY:pack

# sterge executabilul
clean:
	rm -f $(EXE) $(OBJ)

.PHONY:clean

# face coding-style automat, la standardul google, cu o mica modificare
# (4 spatii in loc de 2 la alineate)
beauty: 
	clang-format -i -style=file *.c *.h

.PHONY:beauty

# descarca arhiva de teste si o pregateste
update:
	wget $(CHECKER)
	unzip -o $(CARCHIVE)
	rm -f $(CARCHIVE)*

.PHONY:update

# verifica memoria
MFLAGS = --leak-check=full --show-leak-kinds=all 
memory:
	valgrind $(MFLAGS) ./$(EXE)
.PHONY:memory

prepare:
	cp Readme.md README
.PHONY:prepare