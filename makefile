make: searchfilep2.c
	gcc -pthread -o searchfile searchfilep2.c -lpthread

clean:
	rm -f searchfile
