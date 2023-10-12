#include <winsock.h>
#include <iostream>
#include <sstream>
#include "tcpServer.h"

namespace cpp_http_server{

    TcpServer::TcpServer() {
        WORD versionRequested = MAKEWORD(2,2);
        WSAData wsaData{};
        int err = WSAStartup(versionRequested,&wsaData);

        if(err != 0){
            exitWithError("WSAStartup failed");
        }

        this->tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

        if(this->tcpSocket == INVALID_SOCKET){
            exitWithError("Failed to create socket");
        }

        this->socketAddr.sin_family = AF_INET;
        this->socketAddr.sin_port = 8080;
        this->socketAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

        err = bind(this->tcpSocket, (sockaddr*)&(this->socketAddr), sizeof(this->socketAddr));

        if(err < 0){
            exitWithError("Cannot connect socket to address");
        }
    }

    TcpServer::~TcpServer() {
        closesocket(this->tcpSocket);
        WSACleanup();
    }

    void TcpServer::log(const std::string &message) {
        std::cout << message << std::endl;
    }

    void TcpServer::exitWithError(const std::string &error) {
        std::cout << WSAGetLastError() << std::endl;
        log("ERROR: " + error);
        exit(1);
    }

    void TcpServer::startListen() {
        int err = listen(this->tcpSocket,20);
        if(err < 0){
            exitWithError("Socket listen fail");
        }

        std::ostringstream ss;
        ss << "\n Listening on ADDRESS: " << inet_ntoa(this->socketAddr.sin_addr)
           << " PORT: " << ntohs(this->socketAddr.sin_port)
           << "\n\n";

        log(ss.str());
    }

    void TcpServer::acceptConnection() {
        SOCKET newSocket = accept(this->tcpSocket,(sockaddr*)&(this->socketAddr), (int*)sizeof(this->socketAddr));

        if(newSocket == INVALID_SOCKET){
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from " << inet_ntoa(this->socketAddr.sin_addr)
            << " PORT: " << ntohs(this->socketAddr.sin_port);
            exitWithError(ss.str());
        }
    }

}