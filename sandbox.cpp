// Using C++ on Linux in VS Code https://code.visualstudio.com/docs/cpp/config-linux

#include <cstdarg>
#include <assert.h>
#include <iostream>

#include <unistd.h>
#include <dlfcn.h> // dlopen(3) - Linux man page https://linux.die.net/man/3/dlopen
#include <fcntl.h> // fcntl(2) - Linux man page https://linux.die.net/man/2/fcntl

typedef int (*fcntl_function)(int __fd, int __cmd, ...);

fcntl_function g_fcntl = nullptr;

int fcntl(int fd, int cmd, ...)
{
    if(g_fcntl)
    {
        switch(cmd)
        {
        case F_DUPFD:
            {
                va_list arguments;
                va_start(arguments, cmd);
                auto const another_fd = va_arg(arguments, int);
                va_end(arguments);
                return g_fcntl(fd, cmd, another_fd);
            }
        }
    }
    errno = EFAULT;
    return -1;
}

int main(int argc, char** argv)
{
    char text[128];

    auto const fcntl_address = dlsym(nullptr, "fcntl");
    sprintf(text, "fcntl_address %p", fcntl_address);
    std::cout << text << std::endl;

    auto const library_handle = dlopen("/lib/x86_64-linux-gnu/libc.so.6", RTLD_NOW);
    if(!library_handle)
    {
        std::cout << "dlerror: " << dlerror() << std::endl;
        assert(library_handle);
    }
    auto const library_fcntl_address = dlsym(library_handle, "fcntl");
    auto const library_fcntl64_address = dlsym(library_handle, "fcntl64");
    sprintf(text, "library_handle %p, library_fcntl_address %p, library_fcntl64_address %p", library_handle, library_fcntl_address, library_fcntl64_address);
    std::cout << text << std::endl;

    g_fcntl = reinterpret_cast<fcntl_function>(library_fcntl_address);

    auto const stdout_fd = fileno(stdout);
    assert(stdout_fd > 0);
    auto result = fcntl(stdout_fd, F_DUPFD, stdout_fd);
    assert(result > 0);

    dlclose(library_handle);
    return 0;
}
