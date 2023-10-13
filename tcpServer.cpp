#include <winsock.h>
#include <iostream>
#include <sstream>
#include "tcpServer.h"

namespace cpp_http_server{

    const int BUFFER_SIZE = 30720;

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
        this->socketAddrLen = sizeof(this->socketAddr);
        this->serverMessage = buildResponse();

        err = bind(this->tcpSocket, (sockaddr*)&(this->socketAddr), sizeof(this->socketAddr));

        if(err < 0){
            exitWithError("Cannot connect socket to address");
        }
    }

    void TcpServer::exitWithError(const std::string &error) {
        std::cout << WSAGetLastError() << std::endl;
        log("ERROR: " + error);
        exit(1);
    }

    [[noreturn]] void TcpServer::startListen() {
        int err = listen(this->tcpSocket,20);
        if(err < 0){
            exitWithError("Socket listen fail");
        }

        std::ostringstream ss;
        ss << "\n Listening on ADDRESS: " << inet_ntoa(this->socketAddr.sin_addr)
           << " PORT: " << ntohs(this->socketAddr.sin_port)
           << "\n\n";

        log(ss.str());
        ss.clear();

        int bytesReceived = 0;

        while(true){
            log("====== Waiting for a new connection ======\n\n\n");
            this->acceptConnection();

            char buffer[BUFFER_SIZE] = {0};
            bytesReceived = recv(this->currentSocket,buffer,BUFFER_SIZE,0);
            if(bytesReceived < 0){
                exitWithError("Failed to receive bytes from client socket connection");
            }

            ss << "------ Received Request from client ------\n\n";
            log(ss.str());

            this->sendResponse();

            closesocket(currentSocket);
        }
    }

    void TcpServer::acceptConnection() {
        currentSocket = accept(this->tcpSocket,(sockaddr*)&(this->socketAddr), &this->socketAddrLen);

        if(currentSocket == INVALID_SOCKET){
            std::ostringstream ss;
            ss << "Server failed to accept incoming connection from " << inet_ntoa(this->socketAddr.sin_addr)
            << " PORT: " << ntohs(this->socketAddr.sin_port);
            exitWithError(ss.str());
        }
    }

    std::string TcpServer::buildResponse() {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello World </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void TcpServer::sendResponse() {
        int bytesSent = 0;
        long totalBytesSent = 0;

        while(totalBytesSent < this->serverMessage.size()){
            bytesSent = send(currentSocket, this->serverMessage.c_str(),this->serverMessage.size(),0);

            if(bytesSent < 0){
                break;
            }
            totalBytesSent += bytesSent;
        }

        if(totalBytesSent == this->serverMessage.size()){
            log("------ Server Response sent to client ------\n\n");
        } else {
            log("Error sending response to client.");
        }
    }


    TcpServer::~TcpServer() {
        closesocket(this->tcpSocket);
        WSACleanup();
    }

    void TcpServer::log(const std::string &message) {
        std::cout << message << std::endl;
    }
}