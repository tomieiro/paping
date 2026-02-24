#ifndef PAPING_NET_SOCKET_H
#define PAPING_NET_SOCKET_H

#include "utils/standard.h"

#include "net/host.h"

int socket_resolve(pcc_t hostname, host_t *host);
int socket_set_port_and_type(int port, int type, host_t *host);
int socket_connect(const host_t *host, int timeout_ms, double *time_ms);

int socket_get_socket_type(int type);
pcc_t socket_get_friendly_type_name(int type);

#endif /* PAPING_NET_SOCKET_H */
