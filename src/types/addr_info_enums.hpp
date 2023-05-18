#ifndef TYPES_ADDR_INFO_ENUMS_HPP
#define TYPES_ADDR_INFO_ENUMS_HPP

#include <netdb.h>

enum class AIFlag {
    None = 0,
    Passive = AI_PASSIVE,
    CanonName = AI_CANONNAME,
};

enum class AIFamily {
    IPv4 = AF_INET,
    IPv6 = AF_INET6,
    Unix = AF_UNIX,
    Unspec = AF_UNSPEC,
};

enum class AISockType {
    Stream = SOCK_STREAM,
    Datagram = SOCK_DGRAM,
    SeqPacket = SOCK_SEQPACKET,
    Raw = SOCK_RAW,
    RDM = SOCK_RDM,
};

enum class AIProtocol {
    Any = 0,
    TCP = IPPROTO_TCP,
    UDP = IPPROTO_UDP,
    SCTP = IPPROTO_SCTP,
    Unspec = IPPROTO_IP,
};

#endif // TYPES_ADDR_INFO_ENUMS_HPP