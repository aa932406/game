#include "Other/Answer.h"

#include <sys/socket.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <arpa/inet.h>

int Answer::setkeepalive(socket_t fd, int idleseconds)
{
    int keepalive = 1;
    int keepidle = idleseconds;
    int keepintvl = 10;
    int keepcnt = 5;
    int err = 0;

    err = setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));
    if (err == -1)
        return err;
    err = setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
    if (err == -1)
        return err;
    err = setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
    if (err == -1)
        return err;
    err = setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
    return err;
}

int Answer::setnodelay(socket_t fd)
{
    int optval = 1;
    int err = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
    return err;
}

int Answer::setnonblock(socket_t fd)
{
    unsigned int opts = fcntl(fd, F_GETFL) | O_NONBLOCK;
    int err = fcntl(fd, F_SETFL, opts);
    return err;
}

int Answer::setreuseaddr(socket_t fd)
{
    int val = 1;
    int err = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    return err;
}

int Answer::setreuseport(socket_t fd)
{
    int val = 1;
    int err = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
    return err;
}

int Answer::getreadbuffer(socket_t fd)
{
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen);
    return optval;
}

int Answer::setreadbuffer(socket_t fd, socket_t size)
{
    int err = setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    return err;
}

int Answer::getwritebuffer(socket_t fd)
{
    int optval = 0;
    socklen_t optlen = sizeof(optval);
    getsockopt(fd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen);
    return optval;
}

int Answer::setwritebuffer(socket_t fd, socket_t size)
{
    int err = setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));
    return err;
}
