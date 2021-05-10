main:main.o help.o sniffer.o
	g++ main.o help.o sniffer.o -o  sniffer
main.o:main.cpp
	g++ -c main.cpp
help.o:help.cpp
	g++ -c help.cpp
sniffer.o:sniffer.cpp
	g++ -c sniffer.cpp
.PHONY:clean

#linux 下用 rm -rf *.o main
clean:
	@echo "=======clean project========="
	rm -rf *.o 
	@echo "=======clean completed========="

gdb:
	g++ main.o  -g