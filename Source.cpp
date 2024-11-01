#include <iostream>
#include <winsock2.h>
//Thư viện chứa các định nghĩa cần thiết để sử dụng Winsock API.
#include <ws2tcpip.h>
//Thư viện mở rộng cho Winsock, chứa các định nghĩa cho các giao thức TCP/IP.

#pragma comment(lib, "Ws2_32.lib")

//Chỉ thị cho trình biên dịch liên kết với thư viện Winsock.2 
// Điều này cần thiết để sử dụng các hàm socket.
using namespace std;
// B1..Khởi tạo Winsock
void KhoitaoWinsock() {
    WSADATA wsaData;  //Khai báo một biến wsaData kiểu WSADATA,lưu trữ thông tin trạng thái Winsock
    //WSAStartup: Khởi tạo thư viện Winsock.MAKEWORD(2, 2) yêu cầu phiên bản Winsock 2.2.
    //Nếu khởi tạo thất bại, thông báo lỗi sẽ được in ra và chương trình kết thúc với mã lỗi - 1.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed" << endl;
    }
}
// B2.Tạo socket
SOCKET TaoSocket() {
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Failed to create socket" << endl;
        WSACleanup();
    }
    return serverSocket;
    //Tạo một socket mới.AF_INET chỉ định rằng chúng ta sẽ sử dụng IPv4, 
    // SOCK_STREAM chỉ định rằng socket sẽ sử dụng giao thức TCP.
    // 0 Là giao thức thường dùng của TCP
   //Biến serverSocket sẽ chứa giá trị của socket được tạo
   // Nếu tạo không thành công, nó sẽ có giá trị INVALID_SOCKET.
    //Nếu lỗi, in, dọn dẹp bằng WSACleanup()   rồi kết thúc chương trìn

}
// B3. Chỉ định địa chỉ
sockaddr_in DiaChi(SOCKET &serverSocket) {
    sockaddr_in serverAddress;   //bien nay chua dia chi server
    serverAddress.sin_family = AF_INET;   //sin_family: Chỉ định kiểu địa chỉ(IPv4).
    serverAddress.sin_port = htons(8080);  // sin_port : Chỉ định cổng(8080), sử dụng htons để chuyển đổi số nguyên từ kiểu host sang kiểu network.
    serverAddress.sin_addr.s_addr = INADDR_ANY;   //Chỉ định địa chỉ IP của server. INADDR_ANY cho phép server lắng nghe trên tất cả các địa chỉ IP của máy
    return serverAddress;
}
// B4. Liên kết socket
void LKSocket(SOCKET &serverSocket, sockaddr_in &serverAddress) {
    //bind: Kết nối socket với địa chỉ và cổng đã chỉ định. 
    //Nếu không thành công, in thông báo lỗi, đóng socket và dọn dẹp Winsock.
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Bind failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
    }
}
// B5. Lắng nghe trên socket
void Listen(SOCKET& serverSocket) {
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        //nếu lắng nghe bị lỗi, in,  kết thúc, dọn dẹp
        cerr << "Listen failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return;
    }
    cout << "Server is listening on port 8080..." << endl;
    //In ra thông báo cho biết server đang lắng nghe trên cổng 8080
}
// B6. Chấp nhận yêu cầu kết nối
SOCKET acceptConnection(SOCKET& serverSocket) {

    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
    //accept: Chấp nhận một kết nối đến từ client. 
    // Nếu thành công, biến clientSocket sẽ chứa giá trị của socket đại diện cho kết nối với client.
    if (clientSocket == INVALID_SOCKET) {
        //nếu kết nối không thành công: in, end, clean
        cerr << "Accept failed" << endl;
        closesocket(serverSocket);
        WSACleanup();
    }
    return clientSocket;
}

// B7.Nhận dữ liệu
void NhanDL(SOCKET& clientSocket) {
    char buffer[1024] = { 0 };   //Khai báo một mảng ký tự buffer
    recv(clientSocket, buffer, sizeof(buffer), 0);  //recv: Nhận dữ liệu từ client.Dữ liệu sẽ được lưu vào buffer.
    cout << "Message from client: " << buffer << endl;  //in ra thong diep nhan tu client

}
//Gửi dữ liệu qua client
void GuiDL(SOCKET& clientSocket) {
    const char* message = "Hello, client!";
    send(clientSocket, message, strlen(message), 0);
}

int main() {
    
    KhoitaoWinsock();
   SOCKET serverSocket= TaoSocket();
    
   sockaddr_in serverAddress= DiaChi(serverSocket);
   LKSocket(serverSocket, serverAddress);
   Listen(serverSocket);
   SOCKET clientSocket=acceptConnection(serverSocket);
   NhanDL(clientSocket);
   GuiDL( clientSocket);
 

    // Đóng socket
    closesocket(clientSocket);
    closesocket(serverSocket);
    //Đóng cả socket của client và server
    WSACleanup();
    //WSACleanup() để dọn dẹp tài nguyên mà Winsock đã sử dụng.
    return 0;
}
