#ifndef CPP_HTTP_SERVER_TCPSERVER_H
#define CPP_HTTP_SERVER_TCPSERVER_H

#include <string>
#include <winsock.h>

namespace cpp_http_server{
    class TcpServer {
    public:
        TcpServer();
        ~TcpServer();
    private:
        SOCKET tcpSocket{};
        sockaddr_in socketAddr{};

        void startListen();
        void acceptConnection();
        static void log(const std::string &message);
        static void exitWithError(const std::string &error);
    };
}



#endif //CPP_HTTP_SERVER_TCPSERVER_H
