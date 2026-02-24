#include "net/socket.h"

#include "utils/timer.h"

int socket_resolve(pcc_t hostname, host_t *host)
{
	struct hostent *remote_host;

	if (hostname == NULL || host == NULL) return ERROR_INVALIDARGUMENTS;

	remote_host = NULL;

#ifdef WIN32 /* Init Winsock in Windows */
	{
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) return ERROR_SOCKET_GENERALFAILURE;
	}
#endif

	remote_host = gethostbyname(hostname);

#ifdef WIN32 /* Cleanup Winsock in Windows */
	WSACleanup();
#endif

	if (remote_host == NULL) return ERROR_SOCKET_CANNOTRESOLVE;

	host->IPAddress = *(struct in_addr *)remote_host->h_addr_list[0];
	host->Hostname = remote_host->h_name;
	host->HostIsIP = (strcmp(hostname, host->Hostname) == 0) ? 1 : 0;

	return SUCCESS;
}

int socket_set_port_and_type(int port, int type, host_t *host)
{
	if (host == NULL) return ERROR_INVALIDARGUMENTS;

	host->Port = port;
	host->Type = type;

	return SUCCESS;
}

pcc_t socket_get_friendly_type_name(int type)
{
	switch (type)
	{
		case IPPROTO_TCP:
			return "TCP";
		case IPPROTO_UDP:
			return "UDP";
		default:
			return "UNKNOWN";
	}
}

int socket_get_socket_type(int type)
{
	switch (type)
	{
		case IPPROTO_UDP:
			return SOCK_DGRAM;
		default:
			return SOCK_STREAM;
	}
}

static void socket_fill_timeval_ms(struct timeval *tv, int timeout_ms)
{
	if (tv == NULL) return;

	if (timeout_ms < 0) timeout_ms = 0;
	tv->tv_sec = (long)(timeout_ms / 1000);
	tv->tv_usec = (long)((timeout_ms % 1000) * 1000);
}

int socket_connect(const host_t *host, int timeout_ms, double *time_ms)
{
	int result;
	struct timeval tv;
	paping_timer_t timer;

#ifdef WIN32
	SOCKET client_socket;
	ULONG mode;
#else
	int client_socket;
	long arg;
#endif

	struct sockaddr_in client_address;
	fd_set read_set;
	fd_set write_set;

	if (host == NULL || time_ms == NULL) return ERROR_INVALIDARGUMENTS;

	result = 0;
	*time_ms = 0.0;

#ifdef WIN32 /* Init Winsock in Windows */
	{
		WSADATA wsa_data;
		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) return ERROR_SOCKET_GENERALFAILURE;
	}
#endif

	client_socket = socket(AF_INET, socket_get_socket_type(host->Type), host->Type);

#ifdef WIN32
	if (client_socket == INVALID_SOCKET) return ERROR_SOCKET_GENERALFAILURE;
#else
	if (client_socket == -1) return ERROR_SOCKET_GENERALFAILURE;
#endif

	memset(&client_address, 0, sizeof(client_address));
	client_address.sin_addr = host->IPAddress;
	client_address.sin_family = AF_INET;
	client_address.sin_port = htons((ushort_t)host->Port);

	/* No blocking for Windows/Linux */
#ifdef WIN32
	mode = 1;
	ioctlsocket(client_socket, FIONBIO, &mode);
#else
	arg = fcntl(client_socket, F_GETFL, 0);
	if (arg != -1) (void)fcntl(client_socket, F_SETFL, arg | O_NONBLOCK);
#endif

	socket_fill_timeval_ms(&tv, timeout_ms);

	timer_start(&timer);

	(void)connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address));

	FD_ZERO(&read_set);
	FD_ZERO(&write_set);
	FD_SET(client_socket, &read_set);
	FD_SET(client_socket, &write_set);

#ifdef WIN32
	result = select(0, &read_set, &write_set, NULL, &tv);
#else
	result = select(client_socket + 1, &read_set, &write_set, NULL, &tv);
#endif
	if (result != 1)
	{
		close(client_socket);

#ifdef WIN32 /* Cleanup Winsock in Windows */
		WSACleanup();
#endif

		return ERROR_SOCKET_TIMEOUT;
	}

	*time_ms = timer_stop(&timer);

	if (!FD_ISSET(client_socket, &read_set) && !FD_ISSET(client_socket, &write_set))
	{
		close(client_socket);

#ifdef WIN32 /* Cleanup Winsock in Windows */
		WSACleanup();
#endif

		return ERROR_SOCKET_TIMEOUT;
	}

	close(client_socket);

#ifdef WIN32 /* Cleanup Winsock in Windows */
	WSACleanup();
#endif

	return SUCCESS;
}
