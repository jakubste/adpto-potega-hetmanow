CC		= g++
CFLAGS	= -std=c++11

kompiluj:
	$(CC) $(CFLAGS) adpto.cpp -o adpto
	$(CC) $(CFLAGS) recurrency.cpp -o recurrency

all: kompiluj

clean:
	rm -rf adpto
