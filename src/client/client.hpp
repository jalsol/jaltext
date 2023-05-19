#pragma once

#include <string_view>

#include "types/aliases.hpp"

class TCPClient {
public:
    TCPClient(const std::string_view host, const std::string_view port);

    TCPClient(const TCPClient&) = delete;
    TCPClient(TCPClient&&) = delete;
    TCPClient& operator=(const TCPClient&) = delete;
    TCPClient& operator=(TCPClient&&) = delete;

    ~TCPClient();

    TCPClient& setHost(const std::string_view host);
    TCPClient& setPort(const std::string_view port);

    void run();

private:
    std::string_view m_host;
    std::string_view m_port;

    FileDesc m_sockfd{-1};

    void init();
    void close();
};