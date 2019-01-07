.PHONY:all
all:server client
server:server.cc UdpServer.hpp
	g++ -o $@ $^ -fpermissive -std=c++11 -g 
client:client.cc UdpServer.hpp 
	g++ -o $@ $^ -fpermissive -std=c++11 -g 
.PHONY:clean
clean:
	rm -f server client 

#client:client.cc
