#pragma once

#include <functional>
#include <string_view>
#include <utility>

#include "types/aliases.hpp"
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
    TCPServer& onConnect(
        const std::function<void(FileDesc, const SockAddr&)>& on_connect);

private:
    static constexpr int backlog = 10;

    std::string_view m_host;
    std::string_view m_port;

    FileDesc m_sockfd{-1};

    std::vector<FileDesc> m_clients{};

    std::function<void(FileDesc, const SockAddr&)> m_on_connect{
        [](FileDesc, const SockAddr&) {}};

    void init();

    void send(FileDesc fd, const std::string_view data, int flags);
    std::pair<FileDesc, SockAddr> accept(FileDesc sockfd);
};
