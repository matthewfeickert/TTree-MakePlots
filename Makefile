# makefile targets

all : clean

clean :
	rm -f *.d
	rm -f *.so
	rm -f *.pcm
	\rm -f *~

outputclean: clean
	\rm -f *.pdf

realclean : clean outputclean
	rm -f *.root
