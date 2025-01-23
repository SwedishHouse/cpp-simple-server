#include "request_processing.h"

struct sockaddr_nl sender_addr = {0};

std::string receive_gnl_message(int listen_socket)
{
    struct nlmsghdr *nlh;
    struct genlmsghdr *genlhdr;
    struct iovec iov = {0};
    struct msghdr msg = {0};

    memset(&iov, 0, sizeof(iov));
    memset(&msg, 0, sizeof(msg));

    // Выделяем память для Netlink-сообщения
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
    if (nlh == NULL) {
        perror("Can't allocate memory for netlink message with malloc()");
        return "ERROR";
    }

    memset(nlh, 0, NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
    // Заполняем структуру iovec
    iov.iov_base = (void*)nlh;
    iov.iov_len = NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE);

    // Заполняем структуру msghdr        
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    // Получаем Netlink-сообщение
    ssize_t bytes_received = recvmsg(listen_socket, &msg, 0);
    if (bytes_received < 0) {
        perror("Message receiving Error");
        free(nlh);
        return "ERROR";
    }

    std::cout << "Sender PID: " << nlh->nlmsg_pid << std::endl;

    // Проверяем, что получено Generic Netlink-сообщение
    if (nlh->nlmsg_type == NLMSG_DONE) {
        // Получено пустое сообщение NLMSG_DONE, игнорируем
        printf("Error: empty gnl message received\n");
        free(nlh);
        return "ERROR";
    }

    // Заполнение структуры sockaddr_nl адреса отправителя
    // nlh->nlmsg_pid - PID процесса отправителя
    fill_sockaddr_nl(&sender_addr, AF_NETLINK, nlh->nlmsg_pid, 0);

    // Получено Generic Netlink-сообщение
    genlhdr = (struct genlmsghdr *)NLMSG_DATA(nlh);
    char* buf_for_message = (char*)genlhdr + GENL_HDRLEN;

    std::string res(buf_for_message);
    return res;
}

socklen_t fill_sockaddr_nl(struct sockaddr_nl* addr_nl_struct,
                            short family, pid_t  process_pid, unsigned long groups)
{
    addr_nl_struct->nl_family = family;
    addr_nl_struct->nl_pid = process_pid;
    addr_nl_struct->nl_groups = groups;

    return sizeof(struct sockaddr_nl); 
}

int send_gnl_message(const std::string message, int socket_for_send)
{
    struct nlmsghdr *nlh_;
    struct genlmsghdr *genlhdr_;
    struct iovec iov_ = {0};
    struct msghdr msg_ = {0};

    // Выделяем память для Netlink-сообщения
    nlh_ = (struct nlmsghdr *)malloc(NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE));
    if (nlh_ == NULL) {
        perror("Can't allocate memory for netlink message with malloc()");
        return INCORRECT_VALUE;
    }

    // Заполняем Netlink-сообщение
    nlh_->nlmsg_len = NLMSG_SPACE(GENL_HDRLEN + MAX_MESSAGE_SIZE);
    nlh_->nlmsg_pid = getpid();  // PID отправителя
    nlh_->nlmsg_flags = 0;

    // Заполняем заголовок Generic Netlink
    genlhdr_ = (struct genlmsghdr *)NLMSG_DATA(nlh_);
    genlhdr_->cmd = 1;

    // Копируем данные в Netlink-сообщение
    strcpy((char *)genlhdr_ + GENL_HDRLEN, message.c_str());

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
    ssize_t sent_bytes = sendmsg(socket_for_send, &msg_, 0);
    if (sent_bytes < 0) {
        perror("Error generic netlink message sending");
        free(nlh_);
        return INCORRECT_VALUE;
    }
    printf("Sent bytes:%ld\n", sent_bytes);
    free(nlh_);

    return sent_bytes;
}
