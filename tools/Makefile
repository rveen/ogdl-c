# C=-Wmissing-prototypes -Wstrict-prototypes -I../src
C=-I../src
L=-L../src -logdl

all: 
	gcc ${C} -o gpath    gpath.c    ${L}
	gcc ${C} -o tindent  tindent.c  ${L}
	gcc ${C} -o ogdl2dot  ogdl2dot.c  ${L}
	gcc ${C} -o xml2ogdl xml2ogdl.c ${L} -lexpat
			
clean:
	rm -f *.o *.a gpath tindent xml2ogdl ogdl2dot
	
install:
	cp gpath tindent xml2ogdl ogdl2dot /bin

install-sym:
	ln -sf gpath tindent xml2ogdl ogdl2dot /bin
