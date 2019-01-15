
RM=/bin/rm -f
MV=/bin/mv

INC=../include 

CFLAGS=`libit-config --cflags` -O2 -Wall -I $(INC) 
LFLAGS=`libit-config --libs` -lm

COMMONS=utils.o upsampling.o ss.o

EMBED_OBJ=mixing.o masking.o modulation.o
DECODE_OBJ=fft.o estimation.o

all : embedder decoder

embedder : embedder.o $(COMMONS) $(EMBED_OBJ)
	$(CC) $^ $(LFLAGS) -o $@ 

decoder : decoder.o $(COMMONS) $(DECODE_OBJ)
	$(CC) $^ $(LFLAGS) -o $@ 

.c.o : 
	$(CC) $(CFLAGS) $^ -c 

clean : 
	@$(RM) $(COMMONS) embedder.o decoder.o $(EMBED_OBJ) $(DECODE_OBJ) *~ semantic* 
