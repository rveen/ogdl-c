C=-c -Wmissing-prototypes -Wstrict-prototypes

all: static-lib

static-lib:
	gcc ${C} *.c
	ar -sr libogdl.a *.o

clean:
	rm -f *.o *.a
	rm -f ogdl_wrap.c ogdl.py* _ogdl.so example.py
	
install:
	cp libogdl.a /lib
	cp ogdl.h /usr/include

install-sym:
	ln -sf libogdl.a /lib
	ln -sf ogdl.h /usr/include

swig:
	swig -python -module ogdl ogdl.i
	gcc -c -fPIC -I/usr/local/include/python2.4 *.c
	gcc -shared *.o -o _ogdl.so
pack:
	cd ..; tar cfzh ogdl-python.tgz ogdl-python/
