main:main.o help.o scanner.o
	g++ main.o help.o scanner.o -lpthread -o  scanner
main.o:main.cpp
	g++ -c main.cpp
help.o:help.cpp
	g++ -c help.cpp
scanner.o:scanner.cpp
	g++ -c scanner.cpp
.PHONY:clean

#linux 下用 rm -rf *.o main
clean:
	@echo "=======clean project========="
	rm -rf *.o 
	@echo "=======clean completed========="

gdb:
	g++ main.o help.o scanner.o -lpthread -g