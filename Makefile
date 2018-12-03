# Copyright 2018 Grama Nicolae

CC = gcc
CFLAGS = -lm -Wall -Wextra -std=c99 -O3
EXE = snowfight
SOURCE = source.c

CHECKER = https://ocw.cs.pub.ro/courses/_media/programare/teme_2018/check_gigel.zip
CARCHIVE = check_gigel.zip		# arhiva checker
CFOLDER = ./checker

# arhiva cu tema
ANAME = 312CA_GramaNicolae_Tema3.zip
ACONTENTS = README Makefile *.c *.h
AFLAGS = -FSr 

# compilarea programului
build: $(SOURCE)
		$(CC) -o $(EXE) $^ $(CFLAGS)

# ruleaza programul
run: build
		./$(EXE)

#arhiveaza tema
pack:
	zip $(AFLAGS) $(ANAME) $(ACONTENTS) 
	
.PHONY:pack

# sterge executabilul
clean:
	rm -f $(EXE)

.PHONY:clean

# face coding-style automat, la standardul google, cu o mica modificare
# (4 spatii in loc de 2 la alineate)
beauty: 
	clang-format -i -style=file *.c

.PHONY:beauty

# descarca arhiva de teste si o pregateste
update:
	wget $(CHECKER)
	unzip -o $(CARCHIVE) -d ./checker
	rm -f check_gigel.zip*

.PHONY:update

# verifica doar coding-style
styleCheck: 
	-python $(CFOLDER)/cs.py $(SOURCE)

.PHONY: styleCheck

# pregateste tema pentru verificarea cu checkerul
prepare:
	cp README.md $(CFOLDER)/README.md
	cp main.c $(CFOLDER)/$(SOURCE)
	cp Makefile $(CFOLDER)/Makefile
	
.PHONY: prepare
