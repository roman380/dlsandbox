// Using C++ on Linux in VS Code https://code.visualstudio.com/docs/cpp/config-linux

#include <assert.h>
#include <iostream>

#include <unistd.h>
#include <dlfcn.h> // dlopen(3) - Linux man page https://linux.die.net/man/3/dlopen
#include <fcntl.h> // fcntl(2) - Linux man page https://linux.die.net/man/2/fcntl

int fcntl(int fd, int cmd, ...)
{
    return 0;
}

int main(int argc, char** argv)
{
    char text[128];

    auto const fcntl_address = dlsym(nullptr, "fcntl");
    sprintf(text, "fcntl_address %p", fcntl_address);
    std::cout << text << std::endl;

    auto const library_handle = dlopen(nullptr, RTLD_NOW);
    assert(library_handle);
    auto const library_fcntl_address = dlsym(library_handle, "fcntl");
    auto const library_fcntl64_address = dlsym(library_handle, "fcntl64");
    sprintf(text, "library_handle %p, library_fcntl_address %p, library_fcntl64_address %p", library_handle, library_fcntl_address, library_fcntl64_address);
    std::cout << text << std::endl;

    auto result = fcntl(0, 0, 0);

    dlclose(library_handle);
    return 0;
}
