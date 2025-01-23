#include <iostream>
#include <cstring>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../my-cpp-genetlink/request_processing.h"
#include "pid_manager.h"

#define TEST_NL_BUFF_SIZE 1024
#define GENL_TEST_FAMILY_NAME "genl_test"
#define GENL_TEST_MCGRP_NAME "genl_mcgrp"
#define GENL_TEST_CMD_ECHO 1

int main() {
    unsigned int counter = 0;
    // Create socket
    int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    // Заполнение структуры sockaddr_nl для сервера
    struct sockaddr_nl sa = {0};
    memset(&sa, 0, sizeof(sa)); // Для надежности

    PIDManager pid_mngr;

    pid_t server_pid = getpid();
    pid_mngr.writePIDToFile(pid_info, server_pid);
    std::cout << "Server PID: " << server_pid <<std::endl;
    
    sa.nl_family = AF_NETLINK; // Family Must
    sa.nl_pid = server_pid; //PID MUST
    sa.nl_groups = 0; // In modern must be zero


    //Bind process
    if (bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }
    const std::string response_base = "Response from server: ";
    std::string response; 
    while (true) {
        std::cout << receive_gnl_message(sock_fd) <<std::endl;
        response = response_base + std::to_string(++counter);
        send_gnl_message(response, sock_fd);
        // struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(TEST_NL_BUFF_SIZE));
        // memset(nlh, 0, NLMSG_SPACE(TEST_NL_BUFF_SIZE));

        // struct sockaddr_nl src_addr;
        // socklen_t addr_len = sizeof(src_addr);

        // if (recvfrom(sock_fd, nlh, NLMSG_SPACE(TEST_NL_BUFF_SIZE), 0, (struct sockaddr*)&src_addr, &addr_len) < 0) {
        //     perror("recvfrom");
        //     close(sock_fd);
        //     return -1;
        // }

        // struct genlmsghdr *genlh = (struct genlmsghdr *)NLMSG_DATA(nlh);
        // if (genlh->cmd == GENL_TEST_CMD_ECHO) {
        //     std::cout << "Received message: " << (char *)NLMSG_DATA(nlh) + GENL_HDRLEN << std::endl;
        // }

        // free(nlh);
    }

    close(sock_fd);
    return 0;
}