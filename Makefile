all:
	cd src; make all
	cd tools; make all

doc:
	doxygen doc/doxygen.conf

install:     
	cd src; make install
	cd tools; make install
	
install-sym: 
	cd src; make install-sym
	cd tools; make install-sym
	
clean:
	cd src; make clean
	cd tools; make clean
