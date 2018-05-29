objects= main.o IP_Address.o ping.o parpallel_ping.o
target=scan_host
CXXFLAGS+= -O3 -std=c++11 -pthread
CCC=g++

$(target): $(objects)
		g++ $(objects) -pthread -o $(target)
main.o:
IP_Address.o:
ping.o:
parpallel_ping.o:


.PHONY: clean
	clean:
		rm $(objects)
