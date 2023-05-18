#include "sockaddr.hpp"

#include <types/aliases.hpp>

AIFamily SocketAddress::family() {
    return static_cast<AIFamily>(m_data.ss_family);
}

sockaddr_storage& SocketAddress::data() { return m_data; }

const sockaddr_storage& SocketAddress::data() const { return m_data; }
