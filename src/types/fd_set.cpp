#include "types/fd_set.hpp"

#include "types/aliases.hpp"

void FileDescSet::set(FileDesc fd) {
    FD_SET(fd, &m_set);
    setMaxFD(fd);
}

void FileDescSet::clear(FileDesc fd) { FD_CLR(fd, &m_set); }

bool FileDescSet::isSet(FileDesc fd) const { return FD_ISSET(fd, &m_set); }

void FileDescSet::zero() { FD_ZERO(&m_set); }

fd_set& FileDescSet::data() { return m_set; }

void FileDescSet::setMaxFD(FileDesc fd) {
    if (fd > m_max_fd) {
        m_max_fd = fd;
    }
}

FileDesc FileDescSet::maxFD() const { return m_max_fd; }