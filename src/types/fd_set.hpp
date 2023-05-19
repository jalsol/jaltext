#pragma once

#include <sys/select.h>

#include "types/aliases.hpp"

class FileDescSet {
public:
    FileDescSet() = default;
    ~FileDescSet() = default;

    void set(FileDesc fd);
    void clear(FileDesc fd);
    bool isSet(FileDesc fd) const;

    void zero();

    fd_set& data();

    void setMaxFD(FileDesc fd);
    FileDesc maxFD() const;

private:
    fd_set m_set{};
    FileDesc m_max_fd{-1};
};