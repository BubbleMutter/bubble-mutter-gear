#include <linux/if.h>
#include <linux/if_ether.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

struct rt_notifier_t;

typedef int rt_notifier_parser(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result);
typedef int rt_notifier_handle(struct rt_notifier_t *notifier, const unsigned char *result);

struct rt_notifier_t *rt_notifier_create(int group, rt_notifier_parser *parser, rt_notifier_handle *handle);
void rt_notifier_delete(struct rt_notifier_t *notifier);
int rt_notifier_sockfd(struct rt_notifier_t *notifier);
int rt_notifier_loop(struct rt_notifier_t *notifier, bool wait);
int rt_notifier_stop(struct rt_notifier_t *notifier);
int rt_notifier_dump(struct rt_notifier_t *notifier, int type);

struct rt_notifier_result_link_t {
    int nlmsg_type;
    char ifname[IFNAMSIZ];
    uint32_t ifindex;
    unsigned ifflags;
    uint32_t master;
    char addr[ETH_ALEN];
    uint32_t mtu;
};
int rt_notifier_parser_for_link(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result);

struct rt_notifier_result_neigh_t {
    int reverse;
};
int rt_notifier_parser_for_neigh(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result);

struct rt_notifier_result_route_t {
    int reverse;
};
int rt_notifier_parser_for_route(struct rt_notifier_t *notifier, struct nlmsghdr *nlh, unsigned char *result);