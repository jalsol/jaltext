#include "server/server.hpp"

int main() {
    TCPServer server{"", "8080"};
    server.run();
}