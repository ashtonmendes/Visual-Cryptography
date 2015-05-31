hw2: hw2.o decrypt.o encrypt.o merge.o stream.o
	gcc -g -Wall decrypt.o encrypt.o merge.o stream.o hw2.o -L/home/scf-22/csci551b/openssl/lib -lcrypto -lm -o hw2

hw2.o: hw2.c
	gcc -g -Wall -c hw2.c

decrypt.o: decrypt.c
	gcc -g -Wall -c decrypt.c

encrypt.o: encrypt.c
	gcc -g -Wall -c encrypt.c

merge.o: merge.c
	gcc -g -Wall -c merge.c

stream.o: stream.c
	gcc -g -Wall -c stream.c -I/home/scf-22/csci551b/openssl/include

clean: 
	rm -f *.o hw2
