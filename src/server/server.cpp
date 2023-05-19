#include "server.hpp"

#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include <cstring>
#include <iostream>

#include "jalsock/jalsock.hpp"
#include "types/addr_info.hpp"

TCPServer::TCPServer(const std::string_view host, const std::string_view port)
    : m_host{host}, m_port{port} {}

void TCPServer::init() {
    AddrInfo hints;
    hints.setFlag(AIFlag::Passive)
        .setFamily(AIFamily::Unspec)
        .setSocket(AISockType::Stream);

    const auto& [error, addresses] =
        jalsock::getAddressInfo(m_host, m_port, hints);

    if (error != ErrAI::Success) {
        std::cerr << "Can't get address info: "
                  << gai_strerror(static_cast<int>(error)) << std::endl;
        throw std::runtime_error{gai_strerror(static_cast<int>(error))};
    }

    for (const auto& address : addresses) {
        m_sockfd = jalsock::socket(address.family(), address.sockType(),
                                   address.protocol());

        if (m_sockfd == -1) {
            std::cerr << "Can't create socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        int yes = 1;
        if (jalsock::setSockOpt(m_sockfd, SOL_SOCKET, SockOpt::ReuseAddr, &yes,
                                sizeof(yes)) == -1) {
            std::cerr << "Can't set socket options: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        if (jalsock::bind(m_sockfd, address) == -1) {
            jalsock::close(m_sockfd);
            std::cerr << "Can't bind socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        break;
    }

    if (m_sockfd == -1) {
        throw std::runtime_error{"Can't bind socket"};
    }

    if (jalsock::listen(m_sockfd, backlog) == -1) {
        jalsock::close(m_sockfd);
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
        throw std::runtime_error{"Can't set sigaction"};
    }
}

void TCPServer::run() {
    init();

    std::cerr << "Server is running on " << m_host << ":" << m_port
              << std::endl;

    while (true) {
        SockAddr client_address;
        FileDesc clientfd = jalsock::accept(m_sockfd, client_address);

        std::cerr << "Accepting connection..." << std::endl;

        if (clientfd == -1) {
            std::cerr << "Can't accept connection: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        char ip[INET6_ADDRSTRLEN];
        inet_ntop(static_cast<int>(client_address.family()),
                  jalsock::getInAddr(client_address), ip, INET6_ADDRSTRLEN);

        std::cerr << "Connection from " << ip << std::endl;

        if (jalsock::fork() == 0) {
            jalsock::close(m_sockfd);

            if (jalsock::send(clientfd, "Hello, world!", 0) == -1) {
                std::cerr << "Can't send data: " << std::strerror(errno)
                          << std::endl;
                jalsock::close(clientfd);
                throw std::runtime_error{"Can't send data"};
            }

            jalsock::close(clientfd);
            break;
        }
    }
}

TCPServer& TCPServer::setHost(const std::string_view host) {
    m_host = host;
    return *this;
}

TCPServer& TCPServer::setPort(const std::string_view port) {
    m_port = port;
    return *this;
}
