#pragma once

#include <sys/types.h>

#include <cstddef>
#include <string_view>
#include <vector>

#include "types/aliases.hpp"
#include "types/sockaddr.hpp"

class AddrInfo {
public:
    AddrInfo() = default;
    AddrInfo(const AddrInfo&) = default;
    AddrInfo(const addrinfo&);

    AddrInfo& setFlag(AIFlag flag);
    AddrInfo& setFamily(AIFamily family);
    AddrInfo& setSocket(AISockType sock_type);

    const addrinfo& data() const;
    AIFlag flag() const;
    AIFamily family() const;
    AISockType sockType() const;
    AIProtocol protocol() const;
    sockaddr* address() const;

    socklen_t& addressLen();
    socklen_t addressLen() const;

private:
    addrinfo m_data{};
};
