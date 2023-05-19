#include "jalsock.hpp"

#include <arpa/inet.h>
#include <unistd.h>

namespace jalsock {

void* getInAddr(const SockAddr& address) {
    if (address.family() == AIFamily::IPv4) {
        return &(((sockaddr_in*)&address.data())->sin_addr);
    } else {
        return &(((sockaddr_in6*)&address.data())->sin6_addr);
    }
}

int bind(FileDesc fd, const AddrInfo& address) {
    return ::bind(fd, address.address(), address.addressLen());
}

int listen(FileDesc fd, int backlog) { return ::listen(fd, backlog); }

int accept(FileDesc fd, SockAddr& address) {
    socklen_t size = sizeof(address.data());
    return ::accept(fd, reinterpret_cast<sockaddr*>(&address.data()), &size);
}

int close(FileDesc fd) { return ::close(fd); }

int fork() { return ::fork(); }

int send(FileDesc fd, const std::string_view view, int flags) {
    return ::send(fd, view.data(), view.size(), flags);
}

FileDesc socket(AIFamily domain, AISockType type, AIProtocol protocol) {
    return ::socket(static_cast<int>(domain), static_cast<int>(type),
                    static_cast<int>(protocol));
}

std::pair<ErrAI, std::vector<AddrInfo>> getAddressInfo(
    const std::string_view node, const std::string_view service,
    const AddrInfo& hints) {
    addrinfo* result;
    const char* node_ptr = node.empty() ? nullptr : node.data();

    ErrAI error = static_cast<ErrAI>(
        getaddrinfo(node_ptr, service.data(), &hints.data(), &result));

    std::vector<AddrInfo> addresses;

    for (addrinfo* addr = result; addr != nullptr; addr = addr->ai_next) {
        addresses.push_back(*addr);
    }
    freeaddrinfo(result);

    return {error, addresses};
}

int setSockOpt(FileDesc fd, int level, SockOpt optname, const void* optval,
               socklen_t optlen) {
    return ::setsockopt(fd, level, static_cast<int>(optname), optval, optlen);
}

std::string_view networkToPresentation(const SockAddr& address) {
    static char ipstr[INET6_ADDRSTRLEN];
    inet_ntop(static_cast<int>(address.family()), getInAddr(address), ipstr,
              sizeof(ipstr));
    return ipstr;
}

}  // namespace jalsock
