#include <iostream>
#include <winsock2.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")  //_imp_ 오류 방지 소켓에러 
using namespace std;

#define PACKET_SIZE 1024
SOCKET skt, client_sock;

void proc_recvs() {
    char buffer[PACKET_SIZE] = { 0 };

    while (!WSAGetLastError()) {
        ZeroMemory(&buffer, PACKET_SIZE);
        recv(client_sock, buffer, PACKET_SIZE, 0);
        cout << "받은 메시지: " << buffer << endl;

    }

}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);      //소켓시작

    skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);   //socket(domain , type, protocol)  PF_INET : IPv4체계  SOCK_STREAM(연결지향) : TCP소켓 '순서' 손실x

    SOCKADDR_IN addr = {};              //소켓 주소 구조체
    addr.sin_family = AF_INET;      //주소체계
    addr.sin_port = htons(4444);    //포트
    addr.sin_addr.s_addr = htonl(INADDR_ANY);    //ip 주소

    bind(skt, (SOCKADDR*)&addr, sizeof(addr));       //소켓에 포트번호 부여
    listen(skt, SOMAXCONN);                          //클라이언트 접속을 기다림  

    SOCKADDR_IN client = {};
    int client_size = sizeof(client);
    ZeroMemory(&client, client_size);                //zeromemory 메모리영역을 
    client_sock = accept(skt, (SOCKADDR*)&client, &client_size);      //accept 클라이언트 접속요청 수락함수

    char buffer[PACKET_SIZE] = { 0 };

    thread proc2(proc_recvs);       //thread

    while (!WSAGetLastError()) { //메세지전송
        cin >> buffer;
        send(client_sock, buffer, strlen(buffer), 0);
    }

    proc2.join();
    closesocket(client_sock);
    closesocket(skt);
    WSACleanup();


}