#include "server.hpp"

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
        m_listener = jalsock::socket(address.family(), address.sockType(),
                                     address.protocol());

        if (m_listener == -1) {
            std::cerr << "Can't create socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        int yes = 1;
        if (jalsock::setSockOpt(m_listener, SOL_SOCKET, SockOpt::ReuseAddr,
                                &yes, sizeof(yes)) == -1) {
            std::cerr << "Can't set socket options: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        if (jalsock::bind(m_listener, address) == -1) {
            jalsock::close(m_listener);
            std::cerr << "Can't bind socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        break;
    }

    if (m_listener == -1) {
        throw std::runtime_error{"Can't bind socket"};
    }

    if (jalsock::listen(m_listener, backlog) == -1) {
        jalsock::close(m_listener);
        throw std::runtime_error{"Can't listen socket"};
    }

    m_fd_set.set(m_listener);

    // struct sigaction sa;

    // sa.sa_handler = [](int s) {
    //     int saved_errno = errno;
    //     while (waitpid(-1, NULL, WNOHANG) > 0)
    //         ;
    //     errno = saved_errno;
    // };
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = SA_RESTART;
    // if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    //     perror("sigaction");
    //     throw std::runtime_error{"Can't set sigaction"};
    // }
}

void TCPServer::run() {
    init();

    std::cerr << "Server is running on " << m_host << ":" << m_port
              << std::endl;

    while (true) {
        FileDescSet read_fds = m_fd_set;
        FileDescSet _;

        if (jalsock::select(m_fd_set.maxFD() + 1, read_fds, _, _, nullptr) ==
            -1) {
            std::cerr << "Can't select: " << std::strerror(errno) << std::endl;
            throw std::runtime_error{"Can't select"};
        }

        for (int i = 0; i <= m_fd_set.maxFD(); ++i) {
            if (!read_fds.isSet(i)) {
                continue;
            }

            if (i == m_listener) {
                handleIncoming();
            } else {
                handleRequest(i);
            }
        }
    }
}

void TCPServer::handleIncoming() {
    const auto& [client_fd, client_address] = accept(m_listener);

    if (client_fd == -1) {
        return;
    }

    m_fd_set.set(client_fd);

    std::cerr << "Connection from "
              << jalsock::networkToPresentation(client_address) << " (socket "
              << client_fd << ")" << std::endl;
}

void TCPServer::handleRequest(FileDesc client_fd) {
    const auto& [len, message] = jalsock::recv(client_fd, 0);

    if (len <= 0) {
        if (len == 0) {
            std::cerr << "Disconnected from socket " << client_fd << std::endl;
        } else {
            std::cerr << "Can't receive data: " << std::strerror(errno)
                      << std::endl;
        }

        jalsock::close(client_fd);
        m_fd_set.clear(client_fd);
    } else {
        std::cerr << "Socket " << client_fd << ", message: " << message
                  << std::endl;

        if (message == "/quit") {
            jalsock::close(client_fd);
            m_fd_set.clear(client_fd);
            return;
        }

        for (int other_fd = 0; other_fd <= m_fd_set.maxFD(); ++other_fd) {
            if (m_fd_set.isSet(other_fd) && other_fd != m_listener &&
                other_fd != client_fd) {
                send(other_fd, message, 0);
            }
        }
    }
}

void TCPServer::send(FileDesc clientfd, const std::string_view data,
                     int flags) {
    if (jalsock::send(clientfd, data, flags) == -1) {
        std::cerr << "Can't send data: " << std::strerror(errno) << std::endl;
        jalsock::close(clientfd);
        throw std::runtime_error{"Can't send data"};
    }
}

std::pair<FileDesc, SockAddr> TCPServer::accept(FileDesc sockfd) {
    SockAddr client_address;
    FileDesc clientfd = jalsock::accept(sockfd, client_address);

    std::cerr << "Accepting connection..." << std::endl;

    if (clientfd == -1) {
        std::cerr << "Can't accept connection: " << std::strerror(errno)
                  << std::endl;
    }

    return {clientfd, client_address};
}

TCPServer& TCPServer::setHost(const std::string_view host) {
    m_host = host;
    return *this;
}

TCPServer& TCPServer::setPort(const std::string_view port) {
    m_port = port;
    return *this;
}
