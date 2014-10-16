#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <conio.h>
#include <WinSock2.h>
#include <Windows.h>

#include <thread>
#include <concurrent_queue.h>

#include "../Packet.h"

#define IP "127.0.0.1" //"10.73.45.142"
#define PORT 9916
#define BUF_SIZE 256

using namespace concurrency;
concurrent_queue<char*> g_Queue;

int Recv(const SOCKET sock, char* const buf, const int len)
{
	int recvSize = 0, recvTotal = 0;
	while (recvTotal < len)
	{
		if ((recvSize = recv(sock, buf + recvTotal, len - recvTotal, 0)) == SOCKET_ERROR)
			return SOCKET_ERROR;
		recvTotal += recvSize;
	}
	return recvTotal;
}

void recvThread(SOCKET sock)
{
	char* buf = nullptr;
	int packetSize;

	while (true)
	{
		if (Recv(sock, (char*)&packetSize, 4) == SOCKET_ERROR)
		{
			puts("recv() failed");
			return;
		}
		packetSize -= 4;
		buf = new char[packetSize];
		if (Recv(sock, buf, packetSize) == SOCKET_ERROR)
		{
			puts("recv() failed");
			return;
		}
		g_Queue.push(buf);

	}
}

void main()
{
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		puts("WSAStartup failed");
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		puts("socket failed");
		return;
	}

	sockaddr_in addr = { 0, };
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port = htons(PORT);
	if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		puts("connect() failed");
		return;
	}

	puts("Connected");

	std::thread(recvThread, sock).detach();

	// Game Loop
	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();

			if (key == '\n')
			{
				foo packet;
				packet.a = 1024-5;
				if (send(sock, (char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
				{
					puts("send() failed");
					return;
				}
				puts("foo packet sent");
			}
			else if (key == ' ')
			{
				login_request packet;
				strcpy_s(packet.user_id, sizeof(packet.user_id), "user");
				strcpy_s(packet.user_pw, sizeof(packet.user_pw), "asdf1234");
				if (send(sock, (char*)&packet, sizeof(packet), 0) == SOCKET_ERROR)
				{
					puts("send() failed");
					return;
				}
				puts("login_request packet sent");
			}
		}

		if (!g_Queue.empty())
		{
			int id;
			char* buf = nullptr;
			while(g_Queue.try_pop(buf));
			id = *(int*)buf;

			switch (id)
			{
			case id_login_response:
			{
				login_response packet;
				memcpy_s((char*)&packet + 4, sizeof(packet)-4, buf, sizeof(packet)-4);
				printf("result = %d, nickname = %s\n", packet.result, packet.nickname);
				break;
			}
			case id_bar:
			{
				bar packet;
				memcpy_s((char*)&packet + 4, sizeof(packet)-4, buf, sizeof(packet)-4);
				printf("A = %d\n", packet.a);
				break;
			}
			default:
				break;
			}
			
			
			delete[] buf;

		}

		Sleep(1);

	}

	closesocket(sock);

	WSACleanup();
}
