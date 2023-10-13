#include "tcpServer.h"

int main(){
    using namespace cpp_http_server;

    TcpServer server = TcpServer();
    server.startListen();
    return 0;
}