#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "aliases.hpp"

class SockAddr {
public:
    AIFamily family();
    sockaddr_storage& data();
    const sockaddr_storage& data() const;

private:
    sockaddr_storage m_data{};
};
