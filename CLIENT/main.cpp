#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <conio.h>
using namespace std;
int main(int argc, char** argv) {
	
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0){
		exit(1);
	}
	
	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET,SOCK_STREAM,0);
	if(clientSocket == INVALID_SOCKET){
		cout<<"socket function failed with error = "<< WSAGetLastError()<<endl;
		WSACleanup();
	}
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(50055);
	
	if(connect(clientSocket,(sockaddr *)&addr,sizeof(addr))==-1){
		closesocket(clientSocket);
		WSACleanup();
	}else{
		cout<<"you are connected"<<endl;

	}
	char msg[1500]={0};
	while(1){
		cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg));//clear the buffer
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(clientSocket, (char*)&msg, strlen(msg), 0);
            break;
        }
        send(clientSocket, (char*)&msg, strlen(msg), 0);
        cout << "Awaiting server response..." << endl;
        memset(&msg, 0, sizeof(msg));//clear the buffer
        recv(clientSocket, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Server has quit the session" << endl;
            break;
        }
        cout << "Server: " << msg << endl;

			
	}
//		system("pause");
	close(clientSocket);
//	WSACleanup();
//	getch();
	
	return 0;
}
