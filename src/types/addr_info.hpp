#ifndef TYPES_ADDR_INFO_HPP
#define TYPES_ADDR_INFO_HPP

#include <sys/types.h>
#include <cstddef>
#include <string>

#include "sockaddr.hpp"
#include "addr_info_enums.hpp"

class AddressInfo {
public:

private:
    AIFlag m_flag;
    AIFamily m_family;
    AISockType m_socket;
    AIProtocol m_protocol;

    std::size_t m_length;
    BaseSocketAddress m_address;
    std::string m_canon_name;
};

#endif // TYPES_ADDR_INFO_HPP