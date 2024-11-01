#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h> //b? sung winsock, cho tcp/ip

#pragma comment(lib, "Ws2_32.lib")
//Ch? th? n�y cho tr�nh bi�n d?ch bi?t r?ng b?n mu?n li�n k?t v?i th? vi?n Ws2_32.lib, th? vi?n c?n thi?t cho vi?c s? d?ng c�c h�m c?a Winsock.
using namespace std;
//B1 Kh?i t?o Winsock
void useWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
    }
}
//B2 T?o socket
SOCKET CreatclientSocket() {
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        //khong tao dc nen khoi dong
        WSACleanup();
        return -1;
    }
    return clientSocket;
}
// B3. Ch? ??nh ??a ch? server (cho bi?t ??a ch? server m� client mu?n k?t n?i t?i
sockaddr_in addressServer() {

    sockaddr_in serverAddress;//th�ng tin ??a ch? c?a server m� client s? k?t n?i t?i.
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // ??a ch? localhost
    // cho ph�p client k?t n?i t?i ch�nh m�y t�nh m� n� ?ang ch?y.
    return  serverAddress;
}
//B4. G?i y�u c?u k?t n?i
void requestConnect(SOCKET &clientSocket, sockaddr_in&serverAddress) {
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Connection failed" << endl;
        closesocket(clientSocket);
        WSACleanup();
    }

    //connect: H�m n�y ???c g?i ?? y�u c?u k?t n?i t?i server.
    //clientSocket : Socket m� client s? s? d?ng.
    //sizeof(serverAddress) : K�ch th??c c?a c?u tr�c ??a ch?

}
// B5. G?i d? li?u
void sendMess(SOCKET &clientSocket) {
    const char* message = "Hello, server!";
    //cai clientSocket l� socket ?? k?t n?i client vs server
    send(clientSocket, message, strlen(message), 0);
}

//Nh?n d? li?u t? server
void recMess(SOCKET &clientSocket) {
    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from server: " << buffer << endl;
}
int main() {
    useWinsock();
    SOCKET clientSocket = CreatclientSocket();
    sockaddr_in serverAddress=addressServer();
    requestConnect(clientSocket, serverAddress);
    sendMess(clientSocket);
    recMess( clientSocket);
   


    // ?�ng socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
