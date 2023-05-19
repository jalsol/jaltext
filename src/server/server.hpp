#pragma once

#include <string_view>
#include <utility>
#include <vector>

#include "types/addr_info.hpp"
#include "types/aliases.hpp"

class TCPServer {
public:
    TCPServer(const std::string_view host, const std::string_view port);

    TCPServer(const TCPServer&) = delete;
    TCPServer(TCPServer&&) = delete;
    TCPServer& operator=(const TCPServer&) = delete;
    TCPServer& operator=(TCPServer&&) = delete;

    TCPServer& setHost(const std::string_view host);
    TCPServer& setPort(const std::string_view port);

    void run();

private:
    static constexpr int backlog = 10;

    std::string_view m_host;
    std::string_view m_port;

    FileDesc m_sockfd{-1};

    void init();
};
