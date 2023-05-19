#pragma once

#include <string_view>
#include <utility>

#include "types/aliases.hpp"
#include "types/fd_set.hpp"
#include "types/sockaddr.hpp"

class TCPServer {
public:
    TCPServer(const std::string_view host, const std::string_view port);

    TCPServer(const TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    ~TCPServer(){};

    TCPServer& setHost(const std::string_view host);
    TCPServer& setPort(const std::string_view port);

    void run();

private:
    static constexpr int backlog = 10;

    std::string_view m_host;
    std::string_view m_port;

    FileDesc m_listener{-1};
    FileDescSet m_fd_set{};

    void init();
    void send(FileDesc fd, const std::string_view data, int flags);
    std::pair<FileDesc, SockAddr> accept(FileDesc sockfd);

    void handleIncoming();
    void handleRequest(FileDesc clientfd);
};
