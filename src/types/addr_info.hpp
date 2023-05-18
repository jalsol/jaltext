#ifndef TYPES_ADDR_INFO_HPP
#define TYPES_ADDR_INFO_HPP

#include <sys/types.h>

#include <cstddef>
#include <string_view>
#include <vector>

#include "types/aliases.hpp"
#include "types/sockaddr.hpp"

class AddressInfo {
public:
    AddressInfo() = default;
    AddressInfo(const AddressInfo&) = default;
    AddressInfo(const addrinfo&);

    AddressInfo& setFlag(AIFlag flag);
    AddressInfo& setFamily(AIFamily family);
    AddressInfo& setSocket(AISockType sock_type);

    const addrinfo& data() const;
    AIFlag flag() const;
    AIFamily family() const;
    AISockType sockType() const;
    AIProtocol protocol() const;
    sockaddr* address() const;

    socklen_t& addressLen();
    socklen_t addressLen() const;
    // const BaseSocketAddress& address() const;
    // std::string_view canonName() const;

private:
    addrinfo m_data{};
    // AIFlag m_flag;
    // AIFamily m_family;
    // AISockType m_sock_type;
    // AIProtocol m_protocol;

    // socklen_t m_address_len{};
    // BaseSocketAddress m_address;
    // std::string_view m_canon_name;
};

#endif  // TYPES_ADDR_INFO_HPP