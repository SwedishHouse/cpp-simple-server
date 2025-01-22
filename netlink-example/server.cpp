#include <iostream>
#include <cstring>
#include <unistd.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>

#include "cheat_file.h"

#define GENL_TEST_FAMILY_NAME "genl_test"
#define GENL_TEST_MCGRP_NAME "genl_test_mcgrp"

int main() {
    struct sockaddr_nl sa;
    struct nlmsghdr *nlh;
    FileManager fm(path_to_info);
    int sock_fd;
    char buffer[256];

    std::cout << "Server starts" << std::endl;

    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = getpid();
    sa.nl_groups = 0;

    fm.writeNumber(sa.nl_pid);

    if (bind(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(256));
    while (true) {
        int len = recv(sock_fd, nlh, NLMSG_SPACE(256), 0);
        if (len < 0) {
            perror("recv");
            close(sock_fd);
            return -1;
        }

        std::cout << "Received JSON string: " << (char *)NLMSG_DATA(nlh) + GENL_HDRLEN << std::endl;
    }

    close(sock_fd);
    return 0;
}