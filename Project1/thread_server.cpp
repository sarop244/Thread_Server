#include <iostream>
#include <winsock2.h>
#include <thread>
#pragma comment(lib, "ws2_32.lib")  //_imp_ ���� ���� ���Ͽ��� 
using namespace std;

#define PACKET_SIZE 1024
SOCKET skt, client_sock;

void proc_recvs() {
    char buffer[PACKET_SIZE] = { 0 };

    while (!WSAGetLastError()) {
        ZeroMemory(&buffer, PACKET_SIZE);
        recv(client_sock, buffer, PACKET_SIZE, 0);
        cout << "���� �޽���: " << buffer << endl;

    }

}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);      //���Ͻ���

    skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);   //socket(domain , type, protocol)  PF_INET : IPv4ü��  SOCK_STREAM(��������) : TCP���� '����' �ս�x

    SOCKADDR_IN addr = {};              //���� �ּ� ����ü
    addr.sin_family = AF_INET;      //�ּ�ü��
    addr.sin_port = htons(4444);    //��Ʈ
    addr.sin_addr.s_addr = htonl(INADDR_ANY);    //ip �ּ�

    bind(skt, (SOCKADDR*)&addr, sizeof(addr));       //���Ͽ� ��Ʈ��ȣ �ο�
    listen(skt, SOMAXCONN);                          //Ŭ���̾�Ʈ ������ ��ٸ�  

    SOCKADDR_IN client = {};
    int client_size = sizeof(client);
    ZeroMemory(&client, client_size);                //zeromemory �޸𸮿����� 
    client_sock = accept(skt, (SOCKADDR*)&client, &client_size);      //accept Ŭ���̾�Ʈ ���ӿ�û �����Լ�

    char buffer[PACKET_SIZE] = { 0 };

    thread proc2(proc_recvs);       //thread

    while (!WSAGetLastError()) { //�޼�������
        cin >> buffer;
        send(client_sock, buffer, strlen(buffer), 0);
    }

    proc2.join();
    closesocket(client_sock);
    closesocket(skt);
    WSACleanup();


}