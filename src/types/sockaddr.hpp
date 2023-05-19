#pragma once

#include <sys/socket.h>

#include "types/aliases.hpp"
class SockAddr {
public:
    AIFamily family() const;
    sockaddr_storage& data();
    const sockaddr_storage& data() const;

private:
    sockaddr_storage m_data{};
};
