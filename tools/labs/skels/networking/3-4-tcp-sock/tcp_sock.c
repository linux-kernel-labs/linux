/*
 * SO2 - Networking Lab (#10)
 *
 * Exercise #3, #4: simple kernel TCP socket
 *
 * Code skeleton.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/fs.h>
#include <net/sock.h>

MODULE_DESCRIPTION("Simple kernel TCP socket");
MODULE_AUTHOR("SO2");
MODULE_LICENSE("GPL");

#define LOG_LEVEL		KERN_ALERT
#define MY_TCP_PORT		60000
#define LISTEN_BACKLOG		5

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
				"%pI4:%d\n",	 		\
				&addr.sin_addr.s_addr,		\
				ntohs(addr.sin_port));		\
	} while (0)

static struct socket *sock;	/* listening (server) socket */
static struct socket *new_sock;	/* communication socket */

int __init my_tcp_sock_init(void)
{
	int err;
	/* address to bind on */
	struct sockaddr_in addr = {
		.sin_family	= AF_INET,
		.sin_port	= htons(MY_TCP_PORT),
		.sin_addr	= { htonl(INADDR_LOOPBACK) }
	};
	int addrlen = sizeof(addr);
	/* address of peer */
	struct sockaddr_in raddr;

	/* TODO 1: create listening socket */

	/* TODO 1: bind socket to loopback on port MY_TCP_PORT */

	/* TODO 1: start listening */

	/* TODO 2: create new socket for the accepted connection */

	/* TODO 2: accept a connection */

	/* TODO 2: get the address of the peer and print it */

	return 0;

out_release_new_sock:
	/* TODO 2: cleanup socket for accepted connection */
out_release:
	/* TODO 1: cleanup listening socket */
out:
	return err;
}

void __exit my_tcp_sock_exit(void)
{
	/* TODO 2: cleanup socket for accepted connection */

	/* TODO 1: cleanup listening socket */
}

module_init(my_tcp_sock_init);
module_exit(my_tcp_sock_exit);
