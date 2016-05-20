# makefile targets

all : clean

clean :
	\rm -f *.pdf

realclean : clean
	rm -f *.root
	\rm -f *~
