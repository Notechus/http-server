# Sebastian Paulus 266446

Flags = -std=c++11 -Wextra -g -Wall


server : Makefile server.o utils.o httpserver.o
	g++ $(Flags) -o server server.o utils.o httpserver.o

server.o : Makefile main.cpp utils.o
	g++ -c $(Flags) main.cpp -o  server.o

httpserver.o : Makefile HttpServer.cpp HttpServer.h utils.o
	g++ -c $(Flags) HttpServer.cpp -o httpserver.o

utils.o : Makefile utils.cpp utils.h
	g++ -c $(Flags) utils.cpp -o utils.o

clean:
		rm -rf *.o

distclean:
		rm -rf *.o server .*.swp *.h.gch