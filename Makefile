all: main.o
	g++ main.o -o ttftree

main.o:
	g++ -c main.cpp -o main.o

cleanup:
	rm main.o && rm ttftree

run:
	./ttftree
