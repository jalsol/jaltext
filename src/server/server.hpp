#ifndef SERVER_HPP
#define SERVER_HPP

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

    FileDescriptor m_sockfd{-1};

    int bind(const AddressInfo& address);
    int listen(int backlog);
    int accept(SocketAddress& address);
    int close();
    int fork();
    int send(FileDescriptor sockfd, const std::string_view view, int flags);

    FileDescriptor socket(AIFamily domain, AISockType type,
                          AIProtocol protocol);

    std::pair<ErrorAI, std::vector<AddressInfo>> getAddressInfo(
        const std::string_view node, const std::string_view service,
        const AddressInfo& hints);

    int setsockopt(FileDescriptor sockfd, int level, SocketOption optname,
                   const void* optval, socklen_t optlen);
};

#endif  // SERVER_HPP