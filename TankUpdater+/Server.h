#pragma once
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <WinSock2.h>
    #include <WS2tcpip.h>

#elif __APPLE__ || __linux__
    #include <netinet/in.h>
    #include <unistd.h>
    #include <sys/socket.h>

#endif
class WinServer
{
public:
	int StartServer();
	int StartClient();
};

class UnixServer
{
public:
	int StartServer();
	int StartClient();
};