#pragma once

#include <vector>

#include "types/addr_info.hpp"
#include "types/aliases.hpp"
#include "types/sockaddr.hpp"

namespace jalsock {

int bind(FileDesc fd, const AddrInfo& address);

int listen(FileDesc fd, int backlog);

int accept(FileDesc fd, SockAddr& addr);

int close(FileDesc fd);

int fork();

int send(FileDesc fd, const std::string_view view, int flags);

FileDesc socket(AIFamily domain, AISockType type, AIProtocol protocol);

std::pair<ErrAI, std::vector<AddrInfo>> getAddressInfo(
    const std::string_view node, const std::string_view service,
    const AddrInfo& hints);

int setSockOpt(FileDesc fd, int level, SockOpt optname, const void* optval,
               socklen_t optlen);

}  // namespace jalsock