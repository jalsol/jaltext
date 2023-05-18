#include "server.hpp"

#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "types/addr_info.hpp"

Server::Server(const std::string_view host, const std::string_view port)
    : m_host{host}, m_port{port} {
    AddressInfo hints;
    hints.setFlag(AIFlag::Passive)
        .setFamily(AIFamily::Unspec)
        .setSocket(AISockType::Stream);

    const auto& [error, addresses] = getAddressInfo(m_host, m_port, hints);

    if (error != ErrorAI::Success) {
        std::cerr << "Can't get address info: "
                  << gai_strerror(static_cast<int>(error)) << std::endl;
        throw std::runtime_error{gai_strerror(static_cast<int>(error))};
    }

    bool bound = false;
    for (const auto& address : addresses) {
        m_sockfd =
            socket(address.family(), address.sockType(), address.protocol());

        if (m_sockfd == -1) {
            std::cerr << "Can't create socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        int yes = 1;
        if (setsockopt(m_sockfd, SOL_SOCKET, SocketOption::ReuseAddr, &yes,
                       sizeof(yes)) == -1) {
            std::cerr << "Can't set socket options: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        if (bind(address) == -1) {
            close();
            std::cerr << "Can't bind socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        bound = true;
        break;
    }

    if (!bound) {
        throw std::runtime_error{"Can't bind socket"};
    }

    if (listen(backlog) == -1) {
        close();
        throw std::runtime_error{"Can't listen socket"};
    }

    struct sigaction sa;

    sa.sa_handler = [](int s) {
        int saved_errno = errno;
        while (waitpid(-1, NULL, WNOHANG) > 0)
            ;
        errno = saved_errno;
    };
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
}

void* get_in_addr(const struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Server::run() {
    std::cerr << "Server is running on " << m_host << ":" << m_port
              << std::endl;

    while (true) {
        SocketAddress client_address;
        FileDescriptor clientfd = accept(client_address);

        std::cerr << "Accepting connection..." << std::endl;

        if (clientfd == -1) {
            std::cerr << "Can't accept connection: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        char ip[INET6_ADDRSTRLEN];
        inet_ntop(
            static_cast<int>(client_address.family()),
            get_in_addr(reinterpret_cast<sockaddr*>(&client_address.data())),
            ip, INET6_ADDRSTRLEN);

        std::cerr << "Connection from " << ip << std::endl;

        if (!fork()) {
            close();
            if (send(clientfd, "Hello, world!", 0) == -1) {
                std::cerr << "Can't send data: " << std::strerror(errno)
                          << std::endl;
                ::close(clientfd);
                throw std::runtime_error{"Can't send data"};
            }
            ::close(clientfd);
            break;
        }
    }
}

Server& Server::setHost(const std::string_view host) {
    m_host = host;
    return *this;
}

Server& Server::setPort(const std::string_view port) {
    m_port = port;
    return *this;
}

FileDescriptor Server::socket(AIFamily domain, AISockType type,
                              AIProtocol protocol) {
    return ::socket(static_cast<int>(domain), static_cast<int>(type),
                    static_cast<int>(protocol));
}

std::pair<ErrorAI, std::vector<AddressInfo>> Server::getAddressInfo(
    const std::string_view node, const std::string_view service,
    const AddressInfo& hints) {
    addrinfo* result;
    const char* node_ptr = node.empty() ? nullptr : node.data();

    ErrorAI error = static_cast<ErrorAI>(
        getaddrinfo(node_ptr, service.data(), &hints.data(), &result));

    std::vector<AddressInfo> addresses;

    for (addrinfo* addr = result; addr != nullptr; addr = addr->ai_next) {
        addresses.push_back(*addr);
    }
    freeaddrinfo(result);

    return {error, addresses};
}

int Server::setsockopt(FileDescriptor sockfd, int level, SocketOption optname,
                       const void* optval, socklen_t optlen) {
    return ::setsockopt(sockfd, level, static_cast<int>(optname), optval,
                        optlen);
}

int Server::bind(const AddressInfo& address) {
    return ::bind(m_sockfd, address.address(), address.addressLen());
}

int Server::close() { return ::close(m_sockfd); }

int Server::listen(int backlog) { return ::listen(m_sockfd, backlog); }

int Server::accept(SocketAddress& address) {
    socklen_t size = sizeof(address.data());
    return ::accept(m_sockfd, reinterpret_cast<sockaddr*>(&address.data()),
                    &size);
}

int Server::fork() { return ::fork(); }

int Server::send(FileDescriptor sockfd, const std::string_view view,
                 int flags) {
    return ::send(sockfd, view.data(), view.size(), flags);
}