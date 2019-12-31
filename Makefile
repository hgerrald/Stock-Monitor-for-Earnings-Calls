stocks : main.c process.c
	gcc -Wall -g -I/usr/include/python2.7 main.c process.c -o stocks -lpython2.7 -O0 -ggdb3 -pthread

run:
	./stocks
#$(python-config --includes)
