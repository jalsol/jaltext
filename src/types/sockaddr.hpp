#ifndef TYPES_SOCKADDR_HPP
#define TYPES_SOCKADDR_HPP

#include <array>
#include <cstdint>
#include <memory>

#include "aliases.hpp"

class SocketAddress {
public:
    AIFamily family();
    sockaddr_storage& data();
    const sockaddr_storage& data() const;

private:
    sockaddr_storage m_data{};
};

#endif  // TYPES_SOCKADDR_HPP