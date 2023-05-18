#ifndef TYPES_SOCKADDR_HPP
#define TYPES_SOCKADDR_HPP

#include <cstdint>
#include <array>

#include "addr_info_enums.hpp"

class BaseSocketAddress {
protected:
    AIFamily m_family;
    std::uint16_t m_port;
};

class SocketAddressIPv4 : public BaseSocketAddress {
public:

private:
    std::uint32_t m_address;
};

class SocketAddressIPv6 : public BaseSocketAddress {
public:

private:
    std::array<std::uint8_t, 16> m_address;
    std::uint32_t m_flow_info;
    std::uint32_t m_scope_id;
};

#endif // TYPES_SOCKADDR_HPP