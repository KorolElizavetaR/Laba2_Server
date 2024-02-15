#define PORT 1234

#include <iostream>
#include <ws2tcpip.h>
// #include <winsock2.h>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	WSADATA data;
	WORD DDLversion = MAKEWORD(2, 2);
	if (WSAStartup(DDLversion, &data) != 0) 
	{
		cout << "Start winsock failed." << endl;
		return -1;
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = 0; // inet_addr("127.0.0.1")
	addr.sin_port = htons(PORT);
	addr.sin_family = AF_INET;

	SOCKADDR_IN client_addr;
	int sizeofclient_addr = sizeof(client_addr);

	SOCKET sListen = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (bind(sListen, (SOCKADDR*)&addr, sizeofaddr) == SOCKET_ERROR)
	{
		cout << "Bind socket failed. " << WSAGetLastError() << endl;
		return -1;
	}

	char buf[100];
	const int bufsize = sizeof(buf);

	if (recvfrom(sListen, buf, bufsize, 0, (SOCKADDR*)&client_addr, &sizeofclient_addr) == SOCKET_ERROR)
	{
		cout << "Error received from client. " << WSAGetLastError() << endl;
		return -1;
	}

	/* char buffer[255];
	const int buffersize = sizeof(buffer);
	inet_ntop(AF_INET, &client_addr.sin_addr, buffer, buffersize); */


	closesocket(sListen);
	WSACleanup();
}