#ifndef CPP_HTTP_SERVER_TCPSERVER_H
#define CPP_HTTP_SERVER_TCPSERVER_H

#include <string>
#include <winsock.h>

namespace cpp_http_server{
    class TcpServer {
    public:
        TcpServer();
        ~TcpServer();

        [[noreturn]] void startListen();

    private:
        SOCKET tcpSocket{};
        SOCKET currentSocket{};
        sockaddr_in socketAddr{};
        std::string serverMessage{};

        void acceptConnection();
        std::string buildResponse();
        void sendResponse();
        static void log(const std::string &message);
        static void exitWithError(const std::string &error);
    };
}



#endif //CPP_HTTP_SERVER_TCPSERVER_H
