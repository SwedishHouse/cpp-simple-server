#include <iostream>
#include <cstring>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <sys/socket.h>
#include <unistd.h>

#include "request_processing.h"
#include "pid_manager.h"


#define GENL_TEST_FAMILY_NAME "genl_test"
#define GENL_TEST_MCGRP_NAME "genl_mcgrp"
#define GENL_TEST_CMD_ECHO 1

int main() {
    unsigned int counter = 0;
    int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_GENERIC);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    extern sockaddr_nl sender_addr;
    struct sockaddr_nl client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    memset(&sender_addr, 0, sizeof(sender_addr));
    client_addr.nl_family = AF_NETLINK;


    
    pid_t server_pid, client_pid = getpid();
    std::cout << "Client pid: " << client_pid << std::endl;
    client_addr.nl_family = AF_NETLINK; // Family Must
    client_addr.nl_pid = client_pid; //PID MUST
    client_addr.nl_groups = 0; // In modern must be zero

    if (bind(sock_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind");
        close(sock_fd);
        return -1;
    }
    // It this step, client sockaddr init done
    // Now works with server socket
    PIDManager pid_mngr;
    server_pid = pid_mngr.readPIDFromFile(pid_info);
    std::cout << "Server PID: " << server_pid <<std::endl;

    sender_addr.nl_family = AF_NETLINK; // Family Must
    sender_addr.nl_pid = server_pid; //PID MUST
    sender_addr.nl_groups = 0; // In modern must be zero

    // Init process done (gennl_client_init())
    const std::string request_base = "From client: ";
    std::string response; 
    // Start sending to server
    while (true)
    {
        /* code */
    
    
        struct nlmsghdr *nlh_;
        struct genlmsghdr *genlhdr_;
        struct iovec iov_ = {0};
        struct msghdr msg_ = {0};

        memset(&iov_, 0, sizeof(iov_));
        memset(&msg_, 0, sizeof(msg_));

        // Выделяем память для Netlink-сообщения
        nlh_ = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
        if (nlh_ == NULL) {
            perror("Can't allocate memory for netlink message with malloc()");
            return INCORRECT_VALUE;
        }
        memset(nlh_, 0, NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));

        // Заполняем Netlink-сообщение
        nlh_->nlmsg_len = NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE);
        nlh_->nlmsg_pid = getpid();  // PID отправителя
        nlh_->nlmsg_flags = 0;


        // Заполняем заголовок Generic Netlink
        genlhdr_ = (struct genlmsghdr *)NLMSG_DATA(nlh_);
        genlhdr_->cmd = CMD_CALCULATE;

        response = request_base + std::to_string(++counter);
        // Копируем данные в Netlink-сообщение
        strcpy((char *)genlhdr_ + GENL_HDRLEN, response.c_str());

        // Заполняем структуру iovec
        iov_.iov_base = (void *)nlh_;
        iov_.iov_len = nlh_->nlmsg_len;

        // Заполняем структуру msghdr
        memset(&msg_, 0, sizeof(msg_));
        msg_.msg_name = (void *)&sender_addr;
        msg_.msg_namelen = sizeof(sender_addr);
        msg_.msg_iov = &iov_;
        msg_.msg_iovlen = 1;

        // Отправляем Netlink-сообщение
        ssize_t sent_bytes = sendmsg(sock_fd, &msg_, 0);
        if (sent_bytes < 0) {
            perror("Error generic netlink message sending");
            free(nlh_);
            return INCORRECT_VALUE;
        }
        printf("Sent bytes:%ld\n", sent_bytes);
        free(nlh_);

        std::cout << receive_gnl_message(sock_fd) << std::endl;
        sleep(1);
    }

    // struct nlmsghdr *nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
    // memset(nlh, 0, NLMSG_SPACE(MAX_MESSAGE_SIZE));
    // nlh->nlmsg_len = NLMSG_SPACE(MAX_MESSAGE_SIZE);
    // // pid_t pid_server;
    // // std::cout << "Enter PID: ";
    // // std::cin >> pid_server;
    // // nlh->nlmsg_pid = getpid();
    // nlh->nlmsg_flags = 0;

    // struct genlmsghdr *genlh = (struct genlmsghdr *)NLMSG_DATA(nlh);
    // genlh->cmd = GENL_TEST_CMD_ECHO;
    // genlh->version = 1;

    // strcpy((char *)NLMSG_DATA(nlh) + GENL_HDRLEN, "Hello, Generic Netlink!");

    // struct sockaddr_nl dest_addr;
    // memset(&dest_addr, 0, sizeof(dest_addr));
    // dest_addr.nl_family = AF_NETLINK;
    // dest_addr.nl_pid = pid_server;

    // if (sendto(sock_fd, nlh, nlh->nlmsg_len, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) < 0) {
    //     perror("sendto");
    //     close(sock_fd);
    //     return -1;
    // }

    close(sock_fd);
    return 0;
}