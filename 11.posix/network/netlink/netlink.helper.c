#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>

#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "netlink.helper.h"

#define BUFFER_SIZE (4096) // netlink message normally send a page size
#define RESULT_SIZE (1024) // related parser output, if needed large result, increase it 

struct rt_notifier_t {
    int sockfd;
    rt_notifier_parser *parser; // parser
    rt_notifier_handle *handle; // handle
    void *data;
    struct {
        int error_parser;
        int error_handle;
        int error_alert;
        int error_recv;
        int error_nobufs; /* special error, 
                       * need to increase the recvbuf or 
                       * handle process overload issues */ 
    } counter;
    bool loop;
};

int rt_notifier_sockfd(struct rt_notifier_t *notifier) {
    return notifier->sockfd;
}

/**
 * @brief 
 * @param group bit-wise-or RTMGRP_*
 */
struct rt_notifier_t *rt_notifier_create(int group, rt_notifier_parser *parser, rt_notifier_handle *handle) {
    struct sockaddr_nl sockaddr_nl;

    struct rt_notifier_t *notifier = calloc(1, sizeof(*notifier));
    if (!notifier) {
        MLOG_WARN("rt_notifier notifier calloc (%s)", strerror(errno));
        goto error;
    }
    memset(&sockaddr_nl, 0, sizeof(sockaddr_nl));

    notifier->sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (notifier->sockfd < 0) {
        MLOG_WARN("rt_notifier socket failed (%s)", strerror(errno));
        goto error;
    }

    int opt_val = 16*1024*1024;
    int opt_len = sizeof(opt_val);

    sockaddr_nl.nl_family = AF_NETLINK;
    sockaddr_nl.nl_pid = getpid();
    sockaddr_nl.nl_groups = group;

    if (bind(notifier->sockfd, (struct sockaddr *) &sockaddr_nl, sizeof(sockaddr_nl)) < 0) {
        if (errno != EADDRINUSE) {
            MLOG_WARN("rt_notifier bind failed (%s)", strerror(errno));
            goto error;
        }
    }

    if (setsockopt(notifier->sockfd, SOL_SOCKET, SO_RCVBUF, (char*)&opt_val, opt_len) < 0) {
        MLOG_WARN("rt_notifier setsockopt SO_RCVBUF failed (%s)", strerror(errno));
        goto error;
    }

    MLOG_INFO("rt_notifier created. fd = %d", notifier->sockfd);
    notifier->parser = parser;
    notifier->handle = handle;
    return notifier;
error:
    if (notifier && notifier->sockfd) {
        close(notifier->sockfd);
    }
    return NULL;
}

void rt_notifier_delete(struct rt_notifier_t *notifier) {
    if (notifier) {
        rt_notifier_stop(notifier);
        if (notifier->sockfd) {
            close(notifier->sockfd);
        }
        free(notifier);
    }
}

/**
 * @brief user interface to stop loop
 * @note common used for handle failed
 */
int rt_notifier_stop(struct rt_notifier_t *notifier) {
    notifier->loop = false;
    return 0;
}

/**
 * @note a) receive message failed, will notify `handle' will null-result
 *       b) parse message failed, will not notify `handle'
 *       c) parse message success, will notify `handle' will valid-result
 * @note a) handle failed will continue to parse and handle
 *       b) user who want to stop when handle failed, call rt_notifier_stop
 */
int rt_notifier_loop(struct rt_notifier_t *notifier, bool wait) {
    struct sockaddr_nl sockaddr_nl;
    struct msghdr msg;
    struct iovec iov;
    unsigned char buffer[BUFFER_SIZE /* page size */];
    int ret;
    unsigned char result[RESULT_SIZE];

    notifier->loop = true;
    while (notifier->loop) {
        iov.iov_base = buffer;
        iov.iov_len = sizeof(buffer);
        msg.msg_name = &sockaddr_nl;
        msg.msg_namelen = sizeof(sockaddr_nl);
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        memset(buffer, 0, sizeof(buffer));

        ret = recvmsg(notifier->sockfd, &msg, wait ? 0 : MSG_DONTWAIT);

        if (ret < 0) {
            switch (errno) {
                case EAGAIN:
                    goto end;
                case EINTR:
                    continue;
                case ENOBUFS:
                    notifier->counter.error_nobufs++;
                default:
                    if (notifier->handle(NULL, notifier->data)) {
                        notifier->counter.error_alert++;
                    }
            }
            goto end;
        }
        struct nlmsghdr *nlh = (struct nlmsghdr*) buffer;
        for (nlh = (struct nlmsghdr *)buffer; NLMSG_OK(nlh, ret); nlh = NLMSG_NEXT(nlh, ret)) {
            memset(result, 0, sizeof(result));
            int parser_retval = notifier->parser(notifier, nlh, result);
            if (parser_retval < 0) {
                notifier->counter.error_parser++;
                continue;
            }
            if (!parser_retval && notifier->handle(notifier, result)) {
                notifier->counter.error_handle++;
                continue;
            }
            // paser_retval > 0 means ignore
        }
    }
end:
    return 0;
}

/**
 * @brief request kernel to dump
 * @param type sugguest RTM_GET*
 * @note  using other RTM_* type for dump will cause undefined behavior
 */
int rt_notifier_dump(struct rt_notifier_t *notifier, int type) {
    struct {
        struct nlmsghdr hdr;
        struct rtgenmsg gen;
    } req;
    struct msghdr msg; /* generic msghdr struct for use with sendmsg */
    struct iovec io;   /* IO vector for sendmsg */
    struct sockaddr_nl sockaddr_nl; /* destination */

    memset(&msg, 0, sizeof(msg));
    memset (&req, 0, sizeof(req));
    memset(&sockaddr_nl, 0, sizeof(sockaddr_nl));

    sockaddr_nl.nl_family = AF_NETLINK;
    sockaddr_nl.nl_groups = 0;
   
    req.hdr.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtgenmsg));
    req.hdr.nlmsg_flags = NLM_F_REQUEST | NLM_F_DUMP;
    req.hdr.nlmsg_type = type;
    req.gen.rtgen_family = AF_PACKET;

    io.iov_base = &req;
    io.iov_len = req.hdr.nlmsg_len;
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_name = &sockaddr_nl;
    msg.msg_namelen = sizeof(sockaddr_nl);

    if (sendmsg(notifier->sockfd, (struct msghdr *)&msg, 0) < 0) {
        MLOG_WARN("rt_notifier_dump send failed");
        return -1;
    }
    /* Process the response from kernel */
    MLOG_DBG("rt_notifier_dump send finish");
    return 0;
}

static inline void rt_notifier_rtattr_parser(struct rtattr* buf, int len, const bool cursor[], 
                                             struct rtattr* rtatbl[], int size) {
    for (; RTA_OK(buf, len); buf = RTA_NEXT(buf, len)) {
        uint16_t type = buf->rta_type;
        if (type < size && cursor[type] == true) {
            if (rtatbl[type]) {
                MLOG_INFO("duplicate attr %d\n", type);
            }
            rtatbl[type] = buf;
        }
    }
}

/**
 * @brief parse rtattr of RTM_NEWLINK and RTM_DELLINK
 * @todo support ipv6 address
 * @param result output param, will be cast to rt_notifier_result_link_t;
 */
int rt_notifier_parser_for_link(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result) {
    if (nlh->nlmsg_type != RTM_DELLINK &&
        nlh->nlmsg_type != RTM_NEWLINK) {
        return 1;
    }
    struct rt_notifier_result_link_t *link = (struct rt_notifier_result_link_t *)result;
    struct ifinfomsg *ifinfomsg = NLMSG_DATA(nlh);
    int len = nlh->nlmsg_len - NLMSG_LENGTH(sizeof(*ifinfomsg));

    static const bool cursor[] = {
        [IFLA_IFNAME] = true,
        [IFLA_MASTER] = true,
        [IFLA_MTU]    = true,
        [IFLA_ADDRESS] = true,
    };

    struct rtattr *rtatbl[ARRAY_SIZE(cursor)] = { NULL };
    rt_notifier_rtattr_parser(IFLA_RTA(ifinfomsg), len, cursor, rtatbl, ARRAY_SIZE(cursor));
    if (!rtatbl[IFLA_IFNAME]) {
        MLOG_INFO("missing IFLA_IFNAME");
        return -1;
    }
    strncpy(link->ifname, (char *)RTA_DATA(rtatbl[IFLA_IFNAME]), RTA_PAYLOAD(rtatbl[IFLA_IFNAME]));
    link->mtu    = rtatbl[IFLA_MTU] ? *(uint32_t *)RTA_DATA(rtatbl[IFLA_MTU]) : 0;
    link->master = rtatbl[IFLA_MASTER] ? *(uint32_t *)RTA_DATA(rtatbl[IFLA_MASTER]) : 0;

    if (rtatbl[IFLA_ADDRESS] && RTA_PAYLOAD(rtatbl[IFLA_ADDRESS]) == ETH_ALEN) {
        memcpy(link->addr, (char *)RTA_DATA(rtatbl[IFLA_IFNAME]), ETH_ALEN);
    } else {
        memset(link->addr, 0, ETH_ALEN);
    }

    link->nlmsg_type = nlh->nlmsg_type;
    link->ifindex = ifinfomsg->ifi_index;
    link->ifflags = ifinfomsg->ifi_flags;
    return 0;
}

/**
 * @brief parse RTM_NEWNEIGH RTM_DELNEIGH
 * @todo waited to implement
 */
int rt_notifier_parser_for_neigh(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result) {
    return 0;
}

/**
 * @brief parse RTM_NEWROUTE RTM_DELROUTE
 * @todo waited to implement
 */
int rt_notifier_parser_for_route(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result) {
    return 0;
}