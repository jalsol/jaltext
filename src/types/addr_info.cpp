#include "addr_info.hpp"

#include <sys/socket.h>

#include "aliases.hpp"

AddressInfo::AddressInfo(const addrinfo& data) : m_data{data} {}

AddressInfo& AddressInfo::setFlag(AIFlag flag) {
    m_data.ai_flags = static_cast<int>(flag);
    return *this;
}

AddressInfo& AddressInfo::setFamily(AIFamily family) {
    m_data.ai_family = static_cast<int>(family);
    return *this;
}

AddressInfo& AddressInfo::setSocket(AISockType sock_type) {
    m_data.ai_socktype = static_cast<int>(sock_type);
    return *this;
}

const addrinfo& AddressInfo::data() const { return m_data; }

AIFlag AddressInfo::flag() const {
    return static_cast<AIFlag>(m_data.ai_flags);
}

AIFamily AddressInfo::family() const {
    return static_cast<AIFamily>(m_data.ai_family);
}

AISockType AddressInfo::sockType() const {
    return static_cast<AISockType>(m_data.ai_socktype);
}

AIProtocol AddressInfo::protocol() const {
    return static_cast<AIProtocol>(m_data.ai_protocol);
}

socklen_t& AddressInfo::addressLen() { return m_data.ai_addrlen; }

socklen_t AddressInfo::addressLen() const { return m_data.ai_addrlen; }

sockaddr* AddressInfo::address() const { return m_data.ai_addr; }