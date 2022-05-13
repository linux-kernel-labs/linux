/*
 * SO2 - Networking Lab (#10)
 *
 * Bonus: simple kernel UDP socket
 *
 * Code skeleton.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/in.h>
#include <net/sock.h>

MODULE_DESCRIPTION("Simple kernel UDP socket");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

#define LOG_LEVEL		KERN_ALERT
#define MY_UDP_LOCAL_PORT	60000
#define MY_UDP_REMOTE_PORT	60001
#define MY_TEST_MESSAGE		"kernelsocket\n"

#define ON			1
#define OFF			0
#define DEBUG			ON

#if DEBUG == ON
#define LOG(s)					\
	do {					\
		printk(KERN_DEBUG s "\n");	\
	} while (0)
#else
#define LOG(s)					\
	do {} while (0)
#endif

#define print_sock_address(addr)		\
	do {					\
		printk(LOG_LEVEL "connection established to "	\
				NIPQUAD_FMT ":%d\n", 		\
				NIPQUAD(addr.sin_addr.s_addr),	\
				ntohs(addr.sin_port));		\
	} while (0)

static struct socket *sock;	/* UDP server */

/* send datagram */
static int my_udp_msgsend(struct socket *s)
{
	/* address to send to */
	struct sockaddr_in raddr = {
		.sin_family	= AF_INET,
		.sin_port	= htons(MY_UDP_REMOTE_PORT),
		.sin_addr	= { htonl(INADDR_LOOPBACK) }
	};
	int raddrlen = sizeof(raddr);
	/* message */
	struct msghdr msg;
	struct iovec iov;
	char *buffer = MY_TEST_MESSAGE;
	int len = strlen(buffer) + 1;

	/* TODO 1: build message */

	/* TODO 1: send the message down the socket and return the
	 * error code.
	 */

	return 0;
}

int __init my_udp_sock_init(void)
{
	int err;
	/* address to bind on */
	struct sockaddr_in addr = {
		.sin_family	= AF_INET,
		.sin_port	= htons(MY_UDP_LOCAL_PORT),
		.sin_addr	= { htonl(INADDR_LOOPBACK) }
	};
	int addrlen = sizeof(addr);

	/* TODO 1: create UDP socket */

	/* TODO 1: bind socket to loopback on port MY_UDP_LOCAL_PORT */

	/* send message */
	err = my_udp_msgsend(sock);
	if (err < 0) {
		printk(LOG_LEVEL "can't send message\n");
		goto out_release;
	}

	return 0;

out_release:
	/* TODO 1: release socket */
out:
	return err;
}

void __exit my_udp_sock_exit(void)
{
	/* TODO 1: release socket */
}

module_init(my_udp_sock_init);
module_exit(my_udp_sock_exit);
