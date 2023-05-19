#include "client.hpp"

#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string_view>
#include <utility>

#include "jalsock/jalsock.hpp"
#include "types/addr_info.hpp"

TCPClient::TCPClient(const std::string_view host, const std::string_view port)
    : m_host{host}, m_port{port} {}

TCPClient::~TCPClient() {
    if (m_server_fd != -1) {
        jalsock::send(m_server_fd, "/quit", 0);
        close();
    }
}

TCPClient& TCPClient::setHost(const std::string_view host) {
    m_host = host;
    return *this;
}

TCPClient& TCPClient::setPort(const std::string_view port) {
    m_port = port;
    return *this;
}

void TCPClient::init() {
    AddrInfo hints;
    hints.setFamily(AIFamily::Unspec).setSocket(AISockType::Stream);

    const auto& [error, addresses] =
        jalsock::getAddressInfo(m_host, m_port, hints);

    if (error != ErrAI::Success) {
        std::cerr << "Can't get address info: "
                  << gai_strerror(static_cast<int>(error)) << std::endl;
        throw std::runtime_error{gai_strerror(static_cast<int>(error))};
    }

    for (const auto& address : addresses) {
        m_server_fd = jalsock::socket(address.family(), address.sockType(),
                                      address.protocol());

        if (m_server_fd == -1) {
            std::cerr << "Can't create socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        if (jalsock::connect(m_server_fd, address) == -1) {
            jalsock::close(m_server_fd);
            std::cerr << "Can't connect socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        break;
    }

    if (m_server_fd == -1) {
        throw std::runtime_error{"Can't connect socket"};
    }
}

void TCPClient::run() {
    init();

    std::cerr << "Connected to " << m_host << ":" << m_port << std::endl;

    FileDescSet _;

    while (true) {
        m_fd_set.zero();
        m_fd_set.set(m_server_fd);
        m_fd_set.set(STDIN_FILENO);

        if (jalsock::select(m_server_fd + 1, m_fd_set, _, _, nullptr) == -1) {
            std::cerr << "Can't select: " << std::strerror(errno) << std::endl;
            break;
        }

        if (m_fd_set.isSet(m_server_fd)) {
            const auto& [len, message] = jalsock::recv(m_server_fd, 0);

            if (len == -1) {
                std::cerr << "Can't receive message: " << std::strerror(errno)
                          << std::endl;
                break;
            } else if (len == 0) {
                std::cerr << "Server closed connection" << std::endl;
                break;
            }

            std::cout << message << std::endl;
        }

        if (m_fd_set.isSet(STDIN_FILENO)) {
            std::string message;
            std::getline(std::cin, message);

            if (jalsock::send(m_server_fd, message, 0) == -1) {
                std::cerr << "Can't send message: " << std::strerror(errno)
                          << std::endl;
                break;
            }

            if (message == "/quit") {
                break;
            }
        }
    }

    close();
}

void TCPClient::close() {
    jalsock::close(m_server_fd);
    m_server_fd = -1;
}
