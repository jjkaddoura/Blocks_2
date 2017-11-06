CXX=g++  proj2.cpp image.cpp -ljpeg -lglut -lGL -lGLU -o  

BINARIES=proj2

all: ${BINARIES}
	

clean:
	/bin/rm -f ${BINARIES} *.o *~



