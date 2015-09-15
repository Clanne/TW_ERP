SOURCE = Tw_erp.cpp Tw_erp.h
MYPROGRAM = Tw_erp

CC = g++

all : $(MYPROGRAM)

$(MYPROGRAM) : $(SOURCE)
	$(CC) $(SOURCE) -o$(MYPROGRAM)

# suppression des fichiers.o
clean:
	rm -rf *.o
