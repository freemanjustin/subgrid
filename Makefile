###################################################################
#
# freeman.justin@gmail.com 
#
##################################################################

CC=	gcc

CSRC=	./src/

CFLAGS=	-O0 -g -Wall `/usr/bin/xml2-config --cflags`
#CFLAGS=	-O3 -g -fPIC -Wall

INC=	-I./include

#LFLAGS= -lnetcdf -lxml2 
LFLAGS= `/usr/bin/xml2-config --libs` \
	-lnetcdf

COBJ=	$(CSRC)main.o \
	$(CSRC)readXML.o \
	$(CSRC)jutil.o \
	$(CSRC)fail.o \
	$(CSRC)netcdfIO.o\
	$(CSRC)grid_cut.o\
	$(CSRC)kdtree.o

OBJ=	$(COBJ) 

EXEC=	./bin/subgrid	

$(EXEC):$(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ) $(LFLAGS)

$(COBJ) : %.o : %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	rm $(COBJ)
	rm $(EXEC)
