all: encode decode

encode: Huffman.o Helpers.o encode.o
	g++ -std=c++23 -g -O1 Huffman.o Helpers.o encode.o -o encode

decode: Huffman.o Helpers.o decode.o
	g++ -std=c++23 -g -O1 Huffman.o Helpers.o decode.o -o decode

Huffman.o: Huffman.cxx Huffman.h
	g++ -std=c++23 -g -O1 -c Huffman.cxx

Helpers.o: Helpers.cxx Helpers.h
	g++ -std=c++23 -g -O1 -c Helpers.cxx

encode.o: encode.cxx
	g++ -std=c++23 -g -O1 -c encode.cxx

decode.o: decode.cxx
	g++ -std=c++23 -g -O1 -c decode.cxx

clean:
	rm -rf *.o encode decode

## eof - Makefile
