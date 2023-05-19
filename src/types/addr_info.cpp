#include "addr_info.hpp"

#include <sys/socket.h>

#include "aliases.hpp"

AddrInfo::AddrInfo(const addrinfo& data) : m_data{data} {}

AddrInfo& AddrInfo::setFlag(AIFlag flag) {
    m_data.ai_flags = static_cast<int>(flag);
    return *this;
}

AddrInfo& AddrInfo::setFamily(AIFamily family) {
    m_data.ai_family = static_cast<int>(family);
    return *this;
}

AddrInfo& AddrInfo::setSocket(AISockType sock_type) {
    m_data.ai_socktype = static_cast<int>(sock_type);
    return *this;
}

const addrinfo& AddrInfo::data() const { return m_data; }

AIFlag AddrInfo::flag() const { return static_cast<AIFlag>(m_data.ai_flags); }

AIFamily AddrInfo::family() const {
    return static_cast<AIFamily>(m_data.ai_family);
}

AISockType AddrInfo::sockType() const {
    return static_cast<AISockType>(m_data.ai_socktype);
}

AIProtocol AddrInfo::protocol() const {
    return static_cast<AIProtocol>(m_data.ai_protocol);
}

socklen_t& AddrInfo::addressLen() { return m_data.ai_addrlen; }

socklen_t AddrInfo::addressLen() const { return m_data.ai_addrlen; }

sockaddr* AddrInfo::address() const { return m_data.ai_addr; }