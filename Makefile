stocks : data_control.c process4.c
	gcc -Wall -g -I/usr/include/python2.7 data_control.c process4.c -o stocks -lpython2.7


#$(python-config --includes)
