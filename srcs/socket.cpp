#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9000

void    log(std::string type, std::string msg)
{
    std::cout << "[" << type << "] " << msg << std::endl;
    if (type == "ERROR") exit(EXIT_FAILURE);
}

int main(int argc, char const *argv[])
{
    (void)argc; (void)argv;

    int                 socket_fd, max_fds, ready_fd, opt;
    struct sockaddr_in  addr, client_addr;
    fd_set              main_fds, working_fds;
    char                buffer[2048];

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        log("ERROR", "Failed to create the socket");

    if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) < 0)
        log("ERROR", "Failed to make socket non blocking");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        log("ERROR", "Failed to set socket options");

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        log("ERROR", "Failed to bind socket to port");

    if (listen(socket_fd, SOMAXCONN) < 0)
        log("ERROR", "failed to accept connection on socket");

    std::cout << "🚀 Server listening on http://localhost:" << PORT << std::endl;

    FD_ZERO(&main_fds);
    max_fds = socket_fd;
    FD_SET(socket_fd, &main_fds);

    while (true)
    {
        working_fds = main_fds;
        bzero(buffer, sizeof(buffer));

        std::cout << "🥱 Waiting for a connection" << std::endl;
        if ((ready_fd = select(FD_SETSIZE, &working_fds, NULL, NULL, NULL)) <= 0)
            log("ERROR", "Failed to select");

        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &working_fds))
            {
                if (i == socket_fd)
                {
                    socklen_t client_addr_size;
                    int client_fd = accept(socket_fd, (sockaddr *)&client_addr, &client_addr_size);
                    if (client_fd < 0)
                        log("ERROR", "Failed to create the client socket");
                    std::cout << "[New connection]" << std::endl;
                    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
                        log("ERROR", "Failed to set client socket flags");
                    FD_SET(client_fd, &main_fds);
                    max_fds = client_fd > max_fds ? client_fd : max_fds;
                }
                else
                {
                    std::cout << i << std::endl;
                    ssize_t readSize = recv(i, buffer, sizeof(buffer), 0);
                    std::cout << "=========== [REQUEST]: " << readSize << " ===========\n" << buffer << std::endl;
                    send(i, "HTTP/1.1 200 OK\r\n\r\nWell received\r\n", 33, 0);
                    FD_CLR(i, &main_fds);
                    close(i);
                }
            }
        }
    }

    return (0);
}
