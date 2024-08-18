#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <cstring>
#include <unistd.h>


using namespace std;
int main(int argc, char** argv) {

	WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "WSAStartup failed.\n");
        exit(1);
    }
		
    if (LOBYTE(wsaData.wVersion) != 2 ||
        HIBYTE(wsaData.wVersion) != 2)
    {
        fprintf(stderr,"Version 2.2 of Winsock is not available.\n");
        WSACleanup();
        exit(2);
    }
    
	SOCKET fsocket = INVALID_SOCKET;
	fsocket = socket(AF_INET,SOCK_STREAM,0);
	sockaddr_in addr;	
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port=htons(50055);
	if(bind(fsocket,(struct sockaddr *)&addr,sizeof(addr)) == -1){
		perror("socket");
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	
	if(listen(fsocket,5)==-1){
		perror("listen");
		WSACleanup();
    	exit(EXIT_FAILURE);
	}
	
	
	
	
	
	SOCKET clientSocket = accept(fsocket,NULL,NULL);
	if(clientSocket == INVALID_SOCKET){
		closesocket(fsocket);
		WSACleanup();
		exit(EXIT_FAILURE);
	}else{
		cout << "client connected to the server"<<endl;
	}
	char msg[1500];
	while(1){
        cout << "Awaiting client response..." << endl;
        memset(&msg, 0, sizeof(msg));
        recv(clientSocket, (char*)&msg, sizeof(msg), 0);
        if(!strcmp(msg, "exit"))
        {
            cout << "Client has quit the session" << endl;
            break;
        }
        cout << "Client: " << msg << endl;
        cout << ">";
        string data;
        getline(cin, data);
        memset(&msg, 0, sizeof(msg)); 
        strcpy(msg, data.c_str());
        if(data == "exit")
        {
            send(clientSocket, (char*)&msg, strlen(msg), 0);
            break;
        }
        send(clientSocket, (char*)&msg, strlen(msg), 0);	
	}
	close(fsocket);
	close(clientSocket);
	WSACleanup();

	return 0;
}
