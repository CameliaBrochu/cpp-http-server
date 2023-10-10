#include <winsock.h>
#include <iostream>
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

        err = bind(this->tcpSocket, (sockaddr *)&(this->socketAddr), 0);

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

}