stocks : main.c process4.c isolated_fork.c
	gcc -Wall -g -I/usr/include/python2.7 main.c -o stocks -lpython2.7 -O0 -ggdb3 -pthread

run:
	./stocks
#$(python-config --includes)
