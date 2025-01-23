#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <iostream>
#include <unistd.h>

#define MAX_MESSAGE_SIZE 1024
#define INCORRECT_VALUE -1

#ifndef INCORRECT_VALUE
#define INCORRECT_VALUE -1
#endif

enum CMDS{
    CMD_CALCULATE,
    CMD_MAX
};

std::string receive_gnl_message(int listen_socket);
socklen_t fill_sockaddr_nl(struct sockaddr_nl* addr_nl_struct,
                            short family, pid_t  process_pid, unsigned long groups);
int send_gnl_message(const std::string msg, int socket_for_send);