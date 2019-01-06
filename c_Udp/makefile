.PHONY:all
all:c_server c_client
c_server:c_server.c
	g++ -o $@ $^ -g
c_client:c_client.c
	g++ -o $@ $^ -g
.PHONY:clean
clean:
	rm -f c_server c_client

#client:client.cc
