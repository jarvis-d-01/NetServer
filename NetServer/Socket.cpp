#include "Socket.h"
#include <iostream>
#include <stdio.h>
#include <ws2tcpip.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <cstring>


Socket::Socket() {
	_serverfd = socket(AF_INET, SOCK_STREAM, 0);

	if (-1 == _serverfd) {
		perror("socket create fail");
		exit(-1);
	}
	std::cout << "server create socket" << _serverfd << std::endl;
}

Socket::~Socket() {
	closesocket(_serverfd);
	std::cout << "server close..." << std::endl;
}

void Socket::SetSocketOption() {
	;
}

void Socket::SetReuseAddr() {
	char on = '1';
	setsockopt(_serverfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}

void Socket::Setnonblocking() {
	u_long iMode = 1; // 0->blocking; 1->non-blocking
	int opts = ioctlsocket(_serverfd, FIONBIO, &iMode);
	if (opts < 0) {
		perror("ioctlsocket(_serverfd, FIONBIO, 1)");
		exit(1);
	}

	std::cout << "server setnonblockig..." << std::endl;
}

/*
htonl()--"Host to Network Long"
ntohl()--"Network to Host Long"
htons()--"Host to Network Short"
ntohs()--"Network to Host Short"
*/
bool Socket::BindAddress(int serverport) {
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(serverport);
	int resval = bind(_serverfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (resval == -1) {
		closesocket(_serverfd);
		perror("error bind");
		exit(1);
	}

	std::cout << "srever bindaddress ..." << std::endl;
}

bool Socket::Listen() {
	if (listen(_serverfd, 8192) < 0) {
		perror("error listen");
		closesocket(_serverfd);
		exit(1);
	}

	std::cout << "server listening..." << std::endl;
	return true;
}


int Socket::Accept(struct sockaddr_in &clientaddr) {
	socklen_t lengthofsockaddr = sizeof(clientaddr);
	int clientfd = accept(_serverfd, (struct sockaddr*)&clientaddr, &lengthofsockaddr);
	if (clientfd < 0) {
		if (errno == EAGAIN) {
			return 0;
		}
	}

	return clientfd;
}

bool Socket::Close() {
	closesocket(_serverfd);
	std::cout << "server close..." << std::endl;
	return true;
}
