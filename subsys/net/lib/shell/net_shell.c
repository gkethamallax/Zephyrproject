/** @file
 * @brief Network shell module
 *
 * Provide some networking shell commands that can be useful to applications.
 */

/*
 * Copyright (c) 2016 Intel Corporation
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(net_shell, LOG_LEVEL_DBG);

#include <zephyr/kernel.h>
#include <kernel_internal.h>
#include <zephyr/pm/device.h>
#include <zephyr/random/random.h>
#include <stdlib.h>
#include <stdio.h>
#include <zephyr/shell/shell.h>
#include <zephyr/shell/shell_uart.h>

#include <zephyr/net/net_if.h>
#include <zephyr/sys/printk.h>

#include "common.h"

#include "connection.h"

#if defined(CONFIG_NET_L2_ETHERNET_MGMT)
#include <zephyr/net/ethernet_mgmt.h>
#endif

#if defined(CONFIG_NET_L2_VIRTUAL_MGMT)
#include <zephyr/net/virtual_mgmt.h>
#endif

#if defined(CONFIG_NET_L2_PPP)
#include <zephyr/net/ppp.h>
#include "ppp/ppp_internal.h"
#endif

#include "net_shell.h"

#include <zephyr/sys/fdtable.h>
#include "websocket/websocket_internal.h"

int get_iface_idx(const struct shell *sh, char *index_str)
{
	char *endptr;
	int idx;

	if (!index_str) {
		PR_WARNING("Interface index is missing.\n");
		return -EINVAL;
	}

	idx = strtol(index_str, &endptr, 10);
	if (*endptr != '\0') {
		PR_WARNING("Invalid index %s\n", index_str);
		return -ENOENT;
	}

	if (idx < 0 || idx > 255) {
		PR_WARNING("Invalid index %d\n", idx);
		return -ERANGE;
	}

	return idx;
}

const char *addrtype2str(enum net_addr_type addr_type)
{
	switch (addr_type) {
	case NET_ADDR_ANY:
		return "<unknown type>";
	case NET_ADDR_AUTOCONF:
		return "autoconf";
	case NET_ADDR_DHCP:
		return "DHCP";
	case NET_ADDR_MANUAL:
		return "manual";
	case NET_ADDR_OVERRIDABLE:
		return "overridable";
	}

	return "<invalid type>";
}

const char *addrstate2str(enum net_addr_state addr_state)
{
	switch (addr_state) {
	case NET_ADDR_ANY_STATE:
		return "<unknown state>";
	case NET_ADDR_TENTATIVE:
		return "tentative";
	case NET_ADDR_PREFERRED:
		return "preferred";
	case NET_ADDR_DEPRECATED:
		return "deprecated";
	}

	return "<invalid state>";
}

#if defined(CONFIG_NET_OFFLOAD) || defined(CONFIG_NET_NATIVE)
void get_addresses(struct net_context *context,
		   char addr_local[], int local_len,
		   char addr_remote[], int remote_len)
{
	if (IS_ENABLED(CONFIG_NET_IPV6) && context->local.family == AF_INET6) {
		snprintk(addr_local, local_len, "[%s]:%u",
			 net_sprint_ipv6_addr(
				 net_sin6_ptr(&context->local)->sin6_addr),
			 ntohs(net_sin6_ptr(&context->local)->sin6_port));
		snprintk(addr_remote, remote_len, "[%s]:%u",
			 net_sprint_ipv6_addr(
				 &net_sin6(&context->remote)->sin6_addr),
			 ntohs(net_sin6(&context->remote)->sin6_port));

	} else if (IS_ENABLED(CONFIG_NET_IPV4) && context->local.family == AF_INET) {
		snprintk(addr_local, local_len, "%s:%d",
			 net_sprint_ipv4_addr(
				 net_sin_ptr(&context->local)->sin_addr),
			 ntohs(net_sin_ptr(&context->local)->sin_port));
		snprintk(addr_remote, remote_len, "%s:%d",
			 net_sprint_ipv4_addr(
				 &net_sin(&context->remote)->sin_addr),
			 ntohs(net_sin(&context->remote)->sin_port));

	} else if (context->local.family == AF_UNSPEC) {
		snprintk(addr_local, local_len, "AF_UNSPEC");
	} else if (context->local.family == AF_PACKET) {
		snprintk(addr_local, local_len, "AF_PACKET");
	} else if (context->local.family == AF_CAN) {
		snprintk(addr_local, local_len, "AF_CAN");
	} else {
		snprintk(addr_local, local_len, "AF_UNK(%d)",
			 context->local.family);
	}
}
#endif /* CONFIG_NET_OFFLOAD || CONFIG_NET_NATIVE */

const char *iface2str(struct net_if *iface, const char **extra)
{
#ifdef CONFIG_NET_L2_IEEE802154
	if (net_if_l2(iface) == &NET_L2_GET_NAME(IEEE802154)) {
		if (extra) {
			*extra = "=============";
		}

		return "IEEE 802.15.4";
	}
#endif

#ifdef CONFIG_NET_L2_ETHERNET
	if (net_if_l2(iface) == &NET_L2_GET_NAME(ETHERNET)) {
		if (extra) {
			*extra = "========";
		}

		return "Ethernet";
	}
#endif

#ifdef CONFIG_NET_L2_VIRTUAL
	if (net_if_l2(iface) == &NET_L2_GET_NAME(VIRTUAL)) {
		if (extra) {
			*extra = "=======";
		}

		return "Virtual";
	}
#endif

#ifdef CONFIG_NET_L2_PPP
	if (net_if_l2(iface) == &NET_L2_GET_NAME(PPP)) {
		if (extra) {
			*extra = "===";
		}

		return "PPP";
	}
#endif

#ifdef CONFIG_NET_L2_DUMMY
	if (net_if_l2(iface) == &NET_L2_GET_NAME(DUMMY)) {
		if (extra) {
			*extra = "=====";
		}

		return "Dummy";
	}
#endif

#ifdef CONFIG_NET_L2_OPENTHREAD
	if (net_if_l2(iface) == &NET_L2_GET_NAME(OPENTHREAD)) {
		if (extra) {
			*extra = "==========";
		}

		return "OpenThread";
	}
#endif

#ifdef CONFIG_NET_L2_BT
	if (net_if_l2(iface) == &NET_L2_GET_NAME(BLUETOOTH)) {
		if (extra) {
			*extra = "=========";
		}

		return "Bluetooth";
	}
#endif

#ifdef CONFIG_NET_OFFLOAD
	if (net_if_is_ip_offloaded(iface)) {
		if (extra) {
			*extra = "==========";
		}

		return "IP Offload";
	}
#endif

#ifdef CONFIG_NET_L2_CANBUS_RAW
	if (net_if_l2(iface) == &NET_L2_GET_NAME(CANBUS_RAW)) {
		if (extra) {
			*extra = "==========";
		}

		return "CANBUS_RAW";
	}
#endif

	if (extra) {
		*extra = "==============";
	}

	return "<unknown type>";
}

#if defined(CONFIG_WEBSOCKET_CLIENT)
static void websocket_context_cb(struct websocket_context *context,
				 void *user_data)
{
	struct net_shell_user_data *data = user_data;
	const struct shell *sh = data->sh;
	struct net_context *net_ctx;
	int *count = data->user_data;
	/* +7 for []:port */
	char addr_local[ADDR_LEN + 7];
	char addr_remote[ADDR_LEN + 7] = "";

	net_ctx = z_get_fd_obj(context->real_sock, NULL, 0);
	if (net_ctx == NULL) {
		PR_ERROR("Invalid fd %d", context->real_sock);
		return;
	}

	get_addresses(net_ctx, addr_local, sizeof(addr_local),
		      addr_remote, sizeof(addr_remote));

	PR("[%2d] %p/%p\t%p   %16s\t%16s\n",
	   (*count) + 1, context, net_ctx,
	   net_context_get_iface(net_ctx),
	   addr_local, addr_remote);

	(*count)++;
}
#endif /* CONFIG_WEBSOCKET_CLIENT */

static int cmd_net_websocket(const struct shell *sh, size_t argc,
			     char *argv[])
{
#if defined(CONFIG_WEBSOCKET_CLIENT)
	struct net_shell_user_data user_data;
	int count = 0;

	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	PR("     websocket/net_ctx\tIface         "
	   "Local              \tRemote\n");

	user_data.sh = sh;
	user_data.user_data = &count;

	websocket_context_foreach(websocket_context_cb, &user_data);

	if (count == 0) {
		PR("No connections\n");
	}
#else
	PR_INFO("Set %s to enable %s support.\n", "CONFIG_WEBSOCKET_CLIENT",
		"Websocket");
#endif /* CONFIG_WEBSOCKET_CLIENT */

	return 0;
}

#if defined(CONFIG_NET_SHELL_DYN_CMD_COMPLETION)

SHELL_DYNAMIC_CMD_CREATE(iface_index, iface_index_get);

#endif /* CONFIG_NET_SHELL_DYN_CMD_COMPLETION */



SHELL_STATIC_SUBCMD_SET_CREATE(net_commands,
	SHELL_CMD(websocket, NULL, "Print information about WebSocket "
								"connections.",
		  cmd_net_websocket),
	SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(net_old, &net_commands, "Networking commands", NULL);

/* Placeholder for net commands that are configured in the rest of the .c files */
SHELL_SUBCMD_SET_CREATE(net_cmds, (net));

SHELL_CMD_REGISTER(net, &net_cmds, "Networking commands", NULL);

int net_shell_init(void)
{
	if (IS_ENABLED(CONFIG_NET_MGMT_EVENT_MONITOR_AUTO_START)) {
		events_enable();
	}

	return 0;
}
