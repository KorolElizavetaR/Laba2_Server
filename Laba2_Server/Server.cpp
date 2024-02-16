#define PORT 1234

#include <iostream>
#include <ws2tcpip.h>
// #include <winsock2.h>


#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)

using namespace std;

char* ChangeLine(char* str);
int LineLength(char* str);

int main(int args, char* argv[])
{
	const int requiredlength = 10;

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

	if (recvfrom(sListen, buf, bufsize, 0, (SOCKADDR*)&client_addr, &sizeofclient_addr) == SOCKET_ERROR)//receive line
	{
		cout << "Error recv from client. " << WSAGetLastError() << endl;
		return -1;
	}

	cout << "Message: " << buf << endl;
	
	char sizeofline[3]; int size = LineLength(buf);
	itoa(size, sizeofline, 10);

	if (sendto(sListen, sizeofline, sizeof(sizeofline), 0, (SOCKADDR*)&client_addr, sizeofclient_addr) == SOCKET_ERROR) //send length
	{
		cout << "Sendto client failed. " << WSAGetLastError() << endl;
		return -1;
	}
	else
	{
		cout << "Client received length: " << size << endl;
	}

	/*if (LineLength(buf) == requiredlength)
		ChangeLine(buf);*/
	


	closesocket(sListen);
	WSACleanup();
}

char* ChangeLine(char*str)
{
	char buf[100];
	int poz = 0;
	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			continue;
		buf[poz++] = str[i];
	}
	return buf;
}

int LineLength(char* str)
{
	int counter = 0;
	while (str[counter] != '\0')
		counter++;
	return counter;
}