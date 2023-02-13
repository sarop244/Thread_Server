#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;

#define PACKET_SIZE 1024

SOCKET skt;

void proc_recv() {
	char buffer[PACKET_SIZE] = {};		//char 생성
	string cmd; 		//string 생성
	while (!WSAGetLastError()) {
		ZeroMemory(&buffer, PACKET_SIZE);		//buffer 비우기
		recv(skt, buffer, PACKET_SIZE, 0); 	//데이터 받아오기
		cmd = buffer;	//buffer의 값이 cmd 에 들어간다
		if (cmd == "hi") break;  //cmd의값이 "exit"일 경우 [데이터 받아오기]만 종료
		cout << "받은 메시지:" << buffer << endl;

	}


}


int main() {
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);      //ipv4,tcp 

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;			//주소체계
	addr.sin_port = htons(4444);		//포트
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");		//ip

	while (1) {
		if (!connect(skt, (SOCKADDR*)&addr, sizeof(addr))) break;
	}
	thread proc1(proc_recv);
	//proc1이라는 이름의 thread변수를만들어 "proc_recv"라는함수를 멀티쓰레드로 사용
	char msg[PACKET_SIZE] = { 0 };

	while (!WSAGetLastError()) {			//메시지 전송
		cin >> msg;
		send(skt, msg, strlen(msg), 0);
	}
	proc1.join();
	//proc1이라는 쓰레드변수가 이 함수에서 끝날때까지 기다립니다!
	closesocket(skt);
	WSACleanup();
}