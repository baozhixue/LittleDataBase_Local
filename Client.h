//
// Created by zhixu on 2019/10/31.
//

#ifndef UNTITLED_CLIENT_H
#define UNTITLED_CLIENT_H


#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>

#include "default.h"

#pragma comment(lib, "ws2_32.lib")
struct Client
{
    SOCKET sockClient;
    char send_buff[TOTAL_LENGTH],recv_buff[TOTAL_LENGTH];
    int port = 6666;
    bool WORNING = true;
};

bool Client_init(struct Client * C);
bool Client_Recieve(struct Client * C);
bool Client_Send(struct Client * C);
bool Client_End(struct Client * C);
void Client_run(struct Client * C);

bool Client_init(struct Client * C)
{
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Failed to load Winsock");
        return false;
    }

    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(C->port);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.01");

    //创建套接字
    C->sockClient = socket(AF_INET, SOCK_STREAM, 0);
    if (SOCKET_ERROR == C->sockClient) {
        printf("Socket() error:%d", WSAGetLastError());
        return false;
    }
    if (connect(C->sockClient, (struct  sockaddr*) & addrSrv, sizeof(addrSrv)) == INVALID_SOCKET) {
        printf("Connect failed:%d\n", WSAGetLastError());
        return false;
    }
    return true;
}
bool Client_Recieve(struct Client * C)
{
    recv(C->sockClient, C->recv_buff, sizeof(C->recv_buff), 0);
    if(sizeof(C->recv_buff) == TOTAL_LENGTH && C->recv_buff[MAX_LENGTH] == NEXT_PRINT)
    {
		printf("%s", C->recv_buff);
        memset(C->recv_buff, 0, sizeof(C->recv_buff));
        return true;
    }

	printf("%s", C->recv_buff);
    memset(C->recv_buff, 0, sizeof(C->recv_buff));
    return false;
}
bool Client_Send(struct Client * C)
{
    printf("db > ");
    gets_s(C->send_buff,MAX_LENGTH);
    if(strcmp(C->send_buff,".exit") == 0)
    {
        C->WORNING = false;
    }
    send(C->sockClient,C->send_buff,MAX_LENGTH,0);
    memset(C->send_buff, 0, TOTAL_LENGTH);
    return true;
}
bool Client_End(struct Client * C)
{
    closesocket(C->sockClient);
    WSACleanup();
    return true;
}

void Client_run(struct Client * C)
{
    while (C->WORNING)
    {
        Client_Send(C);
        while (Client_Recieve(C)){}
    }
    Client_End(C);
}

#endif //UNTITLED_CLIENT_H
