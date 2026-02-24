#ifndef PAPING_NET_HOST_H
#define PAPING_NET_HOST_H

#include "utils/standard.h"

typedef struct host_s
{
	pcc_t Hostname;
	struct in_addr IPAddress;
	int HostIsIP;
	int Port;
	int Type;
} host_t;

pcc_t host_ip_address_string(const host_t *host);
int host_format_connect_info(const host_t *host, pc_t out, int out_len);
int host_format_successful_connection(const host_t *host, double time_ms, pc_t out, int out_len);

pcc_t host_get_ip_address_as_string(struct in_addr ip_address);

#endif /* PAPING_NET_HOST_H */
