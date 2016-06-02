CC		= g++
CFLAGS	= -std=c++11

kompiluj:
	$(CC) $(CFLAGS) adpto.cpp -o adpto

all: kompiluj

clean:
	rm -rf adpto