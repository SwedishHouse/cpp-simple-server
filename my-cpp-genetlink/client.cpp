#include <iostream>
#include <cstring>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>
#include <unistd.h>

#define GENL_TEST_FAMILY_NAME "genl_test"
#define GENL_TEST_MCGRP_NAME "genl_mcgrp"
#define GENL_TEST_CMD_ECHO 1

int main() {
    int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    struct sockaddr_nl sa;
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;

    if (bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }

    struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(1024));
    memset(nlh, 0, NLMSG_SPACE(1024));
    nlh->nlmsg_len = NLMSG_SPACE(1024);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    struct genlmsghdr *genlh = (struct genlmsghdr *)NLMSG_DATA(nlh);
    genlh->cmd = GENL_TEST_CMD_ECHO;
    genlh->version = 1;

    strcpy((char *)NLMSG_DATA(nlh) + GENL_HDRLEN, "Hello, Generic Netlink!");

    struct sockaddr_nl dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;

    if (sendto(sock_fd, nlh, nlh->nlmsg_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
        perror("sendto");
        close(sock_fd);
        return -1;
    }

    close(sock_fd);
    return 0;
}