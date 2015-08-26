DOCLEAN=1
ARG=$1
if [ $1 = "clean" ] ; then
	DOCLEAN=0
	ARG=$2
fi

if [ $ARG = "ideas" ] ; then
	cp Makefile.emu.ideas Makefile
	make clean; make
	rm Makefile
elif [ $ARG = "rel" ] ; then
	cp Makefile.ds.release Makefile
	make clean; make
	rm Makefile
elif [ $ARG = "debug" ] ; then
	cp Makefile.ds.debug Makefile
	make clean; make
	rm Makefile
else
	echo "Invalid argument supplied: $1"
fi
