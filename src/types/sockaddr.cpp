#include "sockaddr.hpp"

#include <types/aliases.hpp>

AIFamily SockAddr::family() { return static_cast<AIFamily>(m_data.ss_family); }

sockaddr_storage& SockAddr::data() { return m_data; }

const sockaddr_storage& SockAddr::data() const { return m_data; }
