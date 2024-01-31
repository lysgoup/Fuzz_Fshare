all: fshare fshared

fshare: fshare.c
	gcc -g -o fshare fshare.c

fshared: fshared.c
	gcc -g -o fshared fshared.c -pthread

clean:
	rm -rf fshare fshared