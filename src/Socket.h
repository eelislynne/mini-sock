#pragma once
#define _WINSOCKAPI_

#include <Windows.h>

#include <winsock2.h>
#include <Ws2tcpip.h>

#include <string>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 512

class Socket {
private:
	SOCKET _socket = NULL;
	bool _reconnect = false;
	unsigned int _addr = 0;
	int _port = 0;

	int reconnect() {
		WSADATA wsaData;
		int connected = 0;

		while (connected != 1) {
			int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != NO_ERROR) {
				return iResult;
			}

			connected = init_connection(_addr, _port, true);
			if (connected != 1) {
				Sleep(2000);
			}
			else {
				return NO_ERROR;
			}

		}
	}


public:
	Socket() {

		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			SetLastError(iResult);
		}
	}

	int init_connection(const char* addr, int port, bool reconnect = false) {
		return init_connection(inet_addr(addr), port);
	}

	int init_connection(int addr, int port, bool reconnect = false) {
		_reconnect = reconnect;
		_addr = addr;
		_port = port;

		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET) {
			int error = WSAGetLastError();
			WSACleanup();
			return error;
		}

		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = addr;
		serverAddr.sin_port = htons(port);

		int iResult = connect(_socket, (SOCKADDR*) &serverAddr, sizeof(serverAddr));

		if (iResult == SOCKET_ERROR) {
			closesocket(_socket);
			int error = WSAGetLastError();

			WSACleanup();
			return error;
		}

		return NO_ERROR;
	}

	int close_connection() {
		return closesocket(_socket);
	}

	int send_str(std::string data) {
		return send_data(data.c_str(), data.size());
	}

	int send_data(const char* data) {
		return send_data(data, strlen(data));
	}

	int send_data(const char* data, int len) {
		int iResult = send(_socket, data, len, 0);
		if (iResult == SOCKET_ERROR) {
			closesocket(_socket);
			WSACleanup();

			if (_reconnect)
				reconnect();
		}
		return iResult;
	}

	std::string recv_string() {

		char recvBuf[DEFAULT_BUFLEN];
		int iResult = recv(_socket, recvBuf, DEFAULT_BUFLEN, 0);
		if (iResult == SOCKET_ERROR) {
			closesocket(_socket);
			WSACleanup();

			if (_reconnect)
				reconnect();

			return std::string();
		}

		return std::string(recvBuf);
	}

	int recv_data(char* buf, int len = DEFAULT_BUFLEN) {

		int iResult = recv(_socket, buf, len, 0);
		if (iResult == SOCKET_ERROR) {
			closesocket(_socket);
			WSACleanup();

			if (_reconnect)
				reconnect();
		}

		return iResult;
	}

	unsigned int resolve_domain(const char* host_name) {

		struct hostent* host = gethostbyname(host_name);

		int error = WSAGetLastError();

		if (!host) {
			return NULL;
		}

		return *(unsigned int*)host->h_addr_list[0];
	}

};
