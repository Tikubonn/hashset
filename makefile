
export CFLAGS = -I.

debug: .always 
	make libhashset.a libhashset.so CFLAGS="$(CFLAGS) -Og -g -Wall"

release: .always 
	make libhashset.a libhashset.so CFLAGS="$(CFLAGS) -O3 -Wall"

test: .always
	make debug 
	make test.exe CFLAGS="$(CFLAGS) -Og -g -Wall"
	gdb test.exe 
	#./test.exe

clean: .always 
	rm -f hashset.o

test.exe: test.c libhashset.so hashset.h 
	gcc $(CFLAGS) test.c libhashset.so -o test.exe 

.always:

hashset.o: hashset.c hashset.h 
	gcc $(CFLAGS) -c hashset.c -o hashset.o

libhashset.so: hashset.o 
	gcc $(CFLAGS) -shared hashset.o -o libhashset.so

libhashset.a: hashset.o 
	ar r libhashset.a hashset.o
