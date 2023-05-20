# jaltext: A Made-From-Scratch Texting System

I wanted to get into network programming, so I decided to make a texting system. To make it more interesting, I decided to **make it from scratch.**

I picked C++ because I am most familiar with it. It is low-level enough to allow more control over the system, but high-level enough to allow abstraction without too much hassle.

**This project only relies on the POSIX C library. No external libraries are used.**
It is portable to any POSIX-compliant system (e.g. Linux, macOS, BSD, etc.). The POSIX C library is wrapped in a C++ interface.

## Demonstration

[Here is the demonstration on YouTube.](https://youtu.be/A4diOnhv23I)

## Features
- Multiple clients can send messages to each other
- Non-blocking I/O

## Dependencies

- C++20 (GNU GCC 12.1.0)
- CMake (version 3.22 or above)
- A POSIX-compliant system (e.g. Linux, macOS, BSD, etc.)

## Building

- Clone this repository
- `cmake -S. -Bbuild`
- `make -Cbuild`
- The executables `jaltext_server` and `jaltext_client` in the directory `build/` will appear.

### Extra
- If you can’t build the project, please make sure the dependencies are correctly applied;
- You can use the `-j` flag for Make to enable multithreaded compiling (for example, my laptop has 4 cores, so I usually run `make -Cbuild -j4`).

## Project Structure

- `src/`: Source code of the project implementation
    - `jalsock/`: The wrapped standalone POSIX C library functions (e.g. `send()`, `recv()`, etc.)
    - `types/`: The wrapper classes for the POSIX C library (e.g. `addr_info`, `sockaddr`, etc.)
    - `server/`: The server implementation
    - `client/`: The client implementation

## Notes

- I could have done better with `epoll`, but I decided to use `select` because it is easier to use. I will probably switch to `epoll` in the future.

## License

This project is licensed under the **GNU Affero General Public License v3.0**.