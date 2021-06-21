#ifndef _SOCKET_H_
#define _SOKCET_H_


#include <winsock2.h>
#include <wininet.h>
#include <windows.h>
#pragma comment(lib, "wsock32.lib")

class Socket {
private:
	//socket handle
	int _serverfd;

public:
	Socket();
	~Socket();

	//get handle
	int fd() const { return _serverfd; }

	void SetSocketOption();

	//set reuse address
	void SetReuseAddr();

	void Setnonblocking();
	
	bool BindAddress(int serverport);

	bool Listen();

	int Accept(struct sockaddr_in &clientaddr);

	bool Close();
};

#endif