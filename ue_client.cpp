
#include <iostream>
#include "message.h"
#include "MessageSequencer.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>


using namespace std;

int main () {

	//create the client socket
	int client_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(client_fd < 0) {
		cerr << "Failed to create socket\n";
		return 1;
	}

	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(5050);

	int result = inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

	if (result == 1) {
		cout << "sucessfully written host\n";
	}
	else if (result == 0) {
		cout << "incorrect IPv4\n";
	}
	else {
		cerr << "error occurred\n";
	}

	if(connect(client_fd, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
		cout << "failed to connect\n";
		close(client_fd);
		return 1;
	}

	cout << "Client connected\n";

	// add logs
	MessageSequencer sequence;

	sequence.sequenceLoad();

	while(!sequence.isComplete()){
		Message message;
		// send type length and payload.
		uint16_t type = htons(message.type);
		send(client_fd, &type, sizeof(type), 0);

		uint16_t length = htons(message.length);
		send(client_fd, &length, sizeof(length), 0);

		if(message.length > 0){
			send(client_fd, message.payload.data(), message.payload.size(), 0);
		}

		uint16_t responseType;

		int receivedBytes = recv(client_fd, &responseType, sizeof(responseType), 0);

		if (receivedBytes < 0){
			cerr << "gNodeB disconnected\n";
			break;
		}

		responseType = ntohs(responseType);

		if(!sequence.validateResponse(static_cast<MessageType>(responseType))){
			cerr << "Incorrect message type\n";
		}

		sequence.moveToNext();
	}

	close(client_fd);
	
	return 0;
}
