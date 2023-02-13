#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;

#define PACKET_SIZE 1024

SOCKET skt;

void proc_recv() {
	char buffer[PACKET_SIZE] = {};		//char ����
	string cmd; 		//string ����
	while (!WSAGetLastError()) {
		ZeroMemory(&buffer, PACKET_SIZE);		//buffer ����
		recv(skt, buffer, PACKET_SIZE, 0); 	//������ �޾ƿ���
		cmd = buffer;	//buffer�� ���� cmd �� ����
		if (cmd == "hi") break;  //cmd�ǰ��� "exit"�� ��� [������ �޾ƿ���]�� ����
		cout << "���� �޽���:" << buffer << endl;

	}


}


int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);      //ipv4,tcp 

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;			//�ּ�ü��
	addr.sin_port = htons(4444);		//��Ʈ
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");		//ip

	while (1) {
		if (!connect(skt, (SOCKADDR*)&addr, sizeof(addr))) break;
	}
	thread proc1(proc_recv);
	//proc1�̶�� �̸��� thread����������� "proc_recv"����Լ��� ��Ƽ������� ���
	char msg[PACKET_SIZE] = { 0 };

	while (!WSAGetLastError()) {			//�޽��� ����
		cin >> msg;
		send(skt, msg, strlen(msg), 0);
	}
	proc1.join();
	//proc1�̶�� �����庯���� �� �Լ����� ���������� ��ٸ��ϴ�!
	closesocket(skt);
	WSACleanup();
}