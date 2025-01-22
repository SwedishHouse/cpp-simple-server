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
    struct genlmsghdr *genlh;
    int sock_fd;
    std::string json_str = "{\"message\": \"Hello, Generic Netlink!\"}\0";

    FileManager fm(path_to_info);

    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + json_str.size() + 1));
    nlh->nlmsg_len = NLMSG_LENGTH(GENL_HDRLEN + json_str.size() + 1);
    nlh->nlmsg_type = GENL_ID_CTRL;
    nlh->nlmsg_flags = NLM_F_REQUEST;
    nlh->nlmsg_seq = 0;
    nlh->nlmsg_pid = getpid();

    // nlh->nlmsg_pid = fm.readNumber();

    std::cout << "PID: " << nlh->nlmsg_pid << std::endl;
    genlh = (struct genlmsghdr *)NLMSG_DATA(nlh);
    genlh->cmd = 0;
    genlh->version = 1;

    strcpy((char *)NLMSG_DATA(nlh) + GENL_HDRLEN, json_str.c_str());

    if (sendto(sock_fd, nlh, nlh->nlmsg_len, 0, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("sendto");
        close(sock_fd);
        return -1;
    }

    std::cout << "JSON string sent: " << json_str << std::endl;

    close(sock_fd);
    return 0;
}