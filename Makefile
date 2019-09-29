# constants
CC = g++
# CFLAGS = -I. -Wall -lncurses
# edit to remove unnecessary linking of the ncurses library
CFLAGS = -I. -Wall
OUT = yell

yell: yell.cc
	$(CC) -o $(OUT) $(CFLAGS) yell.cc

configure:
	sudo apt install figlet -y

install: yell configure
	ln -s $(shell pwd)/yell /home/${USER}/.local/bin/yell 

clean:
	rm yell
