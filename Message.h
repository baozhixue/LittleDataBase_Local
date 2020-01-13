#ifndef MESSAGE_H
#define MESSAGE_H

/*
	created by baozhixue in 2020-1-7 ,email is baozhixue@hnu.edu.cn ;


	消息体，在进行远程连接时，消息体将信息发送至目标主机；
			在本地运行时，将信息打印至本地主机显示器。
	
	@@ 统一将程序内的cout更换为Message
*/

#include <iostream>
#include <string>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

#include "DB_STRING.h"
#include "base64.h"

namespace bzx_message {
	enum class RUNNING_MODE { SERVER,USER,LOCAL,TEST };   //默认模式为LOCAL，若选择模式为USER必须连接SERVER
	
	class Server {
	public:
		bool init(int port = 4321);

		int port;
		SOCKET sockSrv, sockConn;
		SOCKADDR_IN addrSrv, addrClient;
		bool RUNNING = false;
	};
	class Client {
	public:
		bool init(int _port = 1234, std::string _inet_addr = "127.0.0.1");

		SOCKADDR_IN addrSrv;
		SOCKET sockSrv;
		int port;
		bool RUNNING = false;
	};

	class Message {
	public:
		Message() = default;
		void OUTPUT(const std::string &str);
		std::string& INPUT(std::string keys = "db>");
		void test_mode(const string& infile_path);
		
		RUNNING_MODE get_rm() {
			return rm;
		}
		bool SERVER(int _port = 6666);
		bool SERVER_Kill(SOCKET _sockConn);
		bool SERVER_NEXT_INPUT();
		bool CLIENT(int _port = 6666, std::string _inet_addr = "127.0.0.1");
		Client client;
		Server server;
	protected:
		RUNNING_MODE rm = RUNNING_MODE::LOCAL;   // 设置默认运行方式
		std::string command_input;
		std::string content_output;
		std::ifstream inFile;
		char send_buf[1024] = { '\0' };
		char recv_buf[1024] = { '\0' };
		base64_bzx::base64 base64;
	};

	bool Server::init(int _port)
	{
		port = _port;
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			std::cout << "Failed to load WinSock!\n";
			return false;
		}
		
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

		sockSrv = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		int retVal = bind(sockSrv, (LPSOCKADDR)&addrSrv, sizeof(SOCKADDR));
		if (retVal == SOCKET_ERROR) {
			std::cout << "Failed bind " << WSAGetLastError()<<"\n";
			return false;
		}
		if (listen(sockSrv, 5) == SOCKET_ERROR)
		{
			std::cout << "Listen failed: " << WSAGetLastError() << "\n";
			return false;
		}

		return true;
	}
	bool Client::init(int _port,std::string _inet_addr)
	{
		WSADATA wsaData;
		
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			printf("Failed to load Winsock");
			return false;
		}
		port = _port;
		addrSrv.sin_family = AF_INET;
		addrSrv.sin_port = htons(port);
		addrSrv.sin_addr.S_un.S_addr = inet_addr(_inet_addr.c_str());

		//创建套接字
		sockSrv = socket(AF_INET, SOCK_STREAM, 0);
		if (SOCKET_ERROR == sockSrv) {
			std::cout << "Socket() error: "<< WSAGetLastError() << "\n";
			return false;
		}
		if (connect(sockSrv, (struct  sockaddr*) & addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			std::cout << "Connect failed: " << WSAGetLastError() << "\n";
			return false;
		}
		return true;
	}

	std::string& Message::INPUT(std::string keys)
	{
		switch (rm)
		{
		case bzx_message::RUNNING_MODE::SERVER:
			/*
				当Client断开链接时，RUNNING为false，则需要等待新链接
			*/
			if(server.RUNNING == false)
			{
				int len = sizeof(SOCKADDR);
				server.sockConn = accept(server.sockSrv, (SOCKADDR*)&server.addrClient, &len);
				if (server.sockConn == SOCKET_ERROR) {
					printf("Accept failed:%d\n", WSAGetLastError());
					cout << 1111111111 << "\n";
					break;
				}
				else {
					server.RUNNING = true;
				}
			}
			recv(server.sockConn, recv_buf, 1024, 0);
			//command_input = recv_buf;
			command_input = base64.decode(recv_buf);
			cout << command_input << "\n";
			break;
		case bzx_message::RUNNING_MODE::USER:
			std::cout << "db>";
			std::cin.getline(send_buf, 1024);
			command_input = send_buf;
			send(client.sockSrv, base64.encode(send_buf), 1024, 0);
			break;
		case bzx_message::RUNNING_MODE::LOCAL:
			std::cout << "db>";
			getline(std::cin, command_input);
			break;
		case bzx_message::RUNNING_MODE::TEST:
			std::cout << "db>";
			getline(inFile, command_input);
			OUTPUT(command_input);
		default:
			break;
		}
		return command_input;
	}

	void Message::OUTPUT(const std::string& str)
	{
		switch (rm)
		{
		case bzx_message::RUNNING_MODE::SERVER:
			memcpy(send_buf, str.c_str(), str.length());
			send_buf[min(str.length(), 1023)] = '\0';
			send(server.sockConn, base64.encode(send_buf), 1024, 0);
			memset(send_buf, '\0', 1024);
			break;
		case bzx_message::RUNNING_MODE::USER:
			while (true)
			{
				recv(client.sockSrv, recv_buf, 1024, 0);
				char* tmp = base64.decode(recv_buf);
				std::cout << tmp << "\n";
				if (strcmp(tmp, "bzx") == 0) { break; }
				if (strcmp(tmp, "Executed!") == 0||
					strcmp(tmp, "Error : Table full.")==0)
				{
					break;
				}
			}
			break;
		case bzx_message::RUNNING_MODE::LOCAL:
		case bzx_message::RUNNING_MODE::TEST:
			std::cout << str <<"\n";
			break;
		default:
			break;
		}
	}

	void Message::test_mode(const string& infile_path)
	{
		// 仅限于本地模式运行
		if (rm != RUNNING_MODE::LOCAL)
		{
			return;
		}
		if (_access(infile_path.c_str(), 0) != 0)
		{
			OUTPUT(infile_path + " is not exist!\n");
		}
		else
		{
			inFile.open(infile_path.c_str(), std::ios::in);
			rm = bzx_message::RUNNING_MODE::TEST;
		}
	}


	bool Message::SERVER(int _port)
	{
		if (server.init(_port) == true)
		{
			rm = RUNNING_MODE::SERVER;
			std::cout << "server is init done!\n";
			return true;
		}
		return false;
	}
	bool Message::SERVER_Kill(SOCKET _sockConn)
	{
		closesocket(server.sockConn);
		server.RUNNING = false;
		return true;
	}
	bool Message::CLIENT(int _port, std::string _inet_addr)
	{
		if (client.init(_port, _inet_addr) == true)
		{
			rm = RUNNING_MODE::USER;
			std::cout << "client is init done!\n";
			return true;
		}
		return false;
	}

	/*
	在创建新表格时需要连续读取内容，此返回使得可以连续读取client发送内容
	*/
	bool Message::SERVER_NEXT_INPUT()
	{
		OUTPUT("bzx");
		return true;
	}
}

#endif // !MESSAGE_H
#pragma once