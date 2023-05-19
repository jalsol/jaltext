#include "client.hpp"

#include <cstring>
#include <iostream>
#include <string_view>
#include <utility>

#include "jalsock/jalsock.hpp"
#include "types/addr_info.hpp"

TCPClient::TCPClient(const std::string_view host, const std::string_view port)
    : m_host{host}, m_port{port} {}

TCPClient::~TCPClient() {
    if (m_sockfd != -1) {
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
        m_sockfd = jalsock::socket(address.family(), address.sockType(),
                                   address.protocol());

        if (m_sockfd == -1) {
            std::cerr << "Can't create socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        if (jalsock::connect(m_sockfd, address) == -1) {
            jalsock::close(m_sockfd);
            std::cerr << "Can't connect socket: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        break;
    }

    if (m_sockfd == -1) {
        throw std::runtime_error{"Can't connect socket"};
    }

    std::cerr << "Connected to " << m_host << ":" << m_port << std::endl;
}

void TCPClient::run() {
    init();

    std::string message;

    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);

        if (jalsock::send(m_sockfd, message, 0) == -1) {
            std::cerr << "Can't send message: " << std::strerror(errno)
                      << std::endl;
            break;
        }

        const auto& [len, response] = jalsock::recv(m_sockfd, 0);
        if (len == -1) {
            std::cerr << "Can't receive message: " << std::strerror(errno)
                      << std::endl;
            continue;
        }

        std::cerr << response << std::endl;

        if (response == "Goodbye!") {
            break;
        }
    }

    close();
}

void TCPClient::close() {
    jalsock::close(m_sockfd);
    m_sockfd = -1;
}
