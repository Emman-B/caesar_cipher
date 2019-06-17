# Makefile
# This is for compiling the program.
# uses g++ and -std=c++14
PROGRAM_NAME = cipher
SOURCE = cipher_main

all:
	g++ -c -std=c++14 ./src/$(SOURCE).cpp
	g++ -o $(PROGRAM_NAME) $(SOURCE).o

clean:
	rm $(PROGRAM_NAME) $(SOURCE).o