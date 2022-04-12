server: ./Tracker_Server/Tracker_Server.c
	gcc -pthread -o ./Tracker_Server/Tracker_Server ./Tracker_Server/Tracker_Server.c -I ./Tracker_Server/include

client: ./Peer_Client/Peer_Client.c
	gcc -pthread -o ./Peer_Client/Peer_Client ./Peer_Client/Peer_Client.c -I ./Peer_Client/include

all: /Tracker_Server/Tracker_Server.c ./Peer_Client/Peer_Client.c
	gcc -pthread -o ./Tracker_Server/Tracker_Server ./Tracker_Server/Tracker_Server.c -I ./Tracker_Server/include
	gcc -pthread -o ./Peer_Client/Peer_Client ./Peer_Client/Peer_Client.c -I ./Peer_Client_1/include