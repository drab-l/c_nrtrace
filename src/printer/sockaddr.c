#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/vm_sockets.h>

#include "util_header_all.h"
#include "sockaddr.h"

#define be_to_h(v_) ((typeof(v_))((sizeof(v_) == 8) ? be64toh(v_) : (sizeof(v_) == 4) ? be32toh(v_) : (sizeof(v_) == 2) ? be16toh(v_) : v_))
void sb_print_sockaddr(printer *pr, void *p, size_t size)
{
#define IS_SOCKADDR_FAMILY_EQ(t_, f_, e_) (((t_*)p)->f_ == e_)
#define IS_SOCKADDR_SIZE_EQ(t_) (size == sizeof(t_))
#define IS_SOCK_TYPE(t_, f_, e_) (IS_SOCKADDR_SIZE_EQ(t_) && IS_SOCKADDR_FAMILY_EQ(t_, f_, e_))
#define IS_INSOCK() IS_SOCK_TYPE(struct sockaddr_in, sin_family, AF_INET)
#define IS_IN6SOCK() IS_SOCK_TYPE(struct sockaddr_in6, sin6_family, AF_INET6)
#define IS_NLSOCK() IS_SOCK_TYPE(struct sockaddr_nl, nl_family, AF_NETLINK)
#define IS_VMSOCK() IS_SOCK_TYPE(struct sockaddr_vm, svm_family, AF_VSOCK)
/* variable length sockaddr */
#define IS_UNSOCK_(t_, f_, e_) (can_access_member(t_, f_, size) && IS_SOCKADDR_FAMILY_EQ(t_, f_, e_))
#define IS_UNSOCK() IS_UNSOCK_(struct sockaddr_un, sun_family, AF_UNIX)

    string_buffer *sb = pr->sb;
    if (IS_UNSOCK()) {
        struct sockaddr_un *s = p;
        if (size <= offsetof(struct sockaddr_un, sun_path)) {
            string_buffer_strcat(sb, ".sun_family = AF_UNIX");
        } else if (s->sun_path[0] == '\0') {
            string_buffer_strcat(sb, ".sun_family = AF_UNIX, .sun_path = \"\\x00");
            string_buffer_strcat(sb, &s->sun_path[1]);
            string_buffer_charcat(sb, '"');
        } else {
            string_buffer_strcat(sb, ".sun_family = AF_UNIX, .sun_path = \"");
            string_buffer_strcat(sb, s->sun_path);
            string_buffer_charcat(sb, '"');
        }
    } else if (IS_INSOCK()) {
        char addr[INET_ADDRSTRLEN];
        struct sockaddr_in *s = p;
        inet_ntop(AF_INET, s, addr, sizeof(addr));
        string_buffer_strcat(sb, ".sin_family = AF_INET, .sin_port = hton(");
        string_buffer_dec(sb, be_to_h(s->sin_port));
        string_buffer_strcat(sb, "), .sin_addr = {.s_addr = ");
        string_buffer_strcat(sb, addr);
        string_buffer_strcat(sb, "}");
    } else if (IS_IN6SOCK()) {
        char addr[INET6_ADDRSTRLEN];
        struct sockaddr_in6 *s = p;
        inet_ntop(AF_INET6, s, addr, sizeof(addr));
        string_buffer_strcat(sb, ".sin6_family = AF_INET6, .sin6_port = hton(");
        string_buffer_dec(sb, be32toh(s->sin6_port));
        string_buffer_strcat(sb, "), .sin6_flowinfo = ");
        string_buffer_dec(sb, s->sin6_flowinfo);
        string_buffer_strcat(sb, ", .sin6_addr = {.s6_addr = ");
        string_buffer_strcat(sb, addr);
        string_buffer_strcat(sb, "}, .sin6_scope_id = ");
        string_buffer_dec(sb, s->sin6_scope_id);
    } else if (IS_NLSOCK()) {
        struct sockaddr_nl *s = p;
        string_buffer_strcat(sb, ".nl_family = AF_NETLINK, .nl_pad = ");
        string_buffer_hex(sb, s->nl_pad);
        string_buffer_strcat(sb, ", .nl_pid = ");
        string_buffer_dec(sb, s->nl_pid);
        string_buffer_strcat(sb, ", .nl_groups = ");
        string_buffer_dec(sb, s->nl_groups);
    } else if (IS_VMSOCK()) {
        struct sockaddr_vm *s = p;
        string_buffer_strcat(sb, ".svm_family = AF_VSOCK, .svm_port = ");
        string_buffer_dec(sb, s->svm_port);
        string_buffer_strcat(sb, ", .svm_cid = ");
        string_buffer_dec(sb, s->svm_cid);
    } else {
        string_buffer_hexcat(sb, p, size);
    }
}
