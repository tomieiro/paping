#include "net/host.h"

#include "cli/i18n.h"
#include "net/socket.h"

pcc_t host_ip_address_string(const host_t *host)
{
	if (host == NULL) return NULL;
	return host_get_ip_address_as_string(host->IPAddress);
}

int host_format_successful_connection(const host_t *host, double time_ms, pc_t out, int out_len)
{
	pcc_t format;

	if (host == NULL || out == NULL || out_len <= 0) return ERROR_INVALIDARGUMENTS;

	format = i18n_get_string(STRING_CONNECT_SUCCESS);
	snprintf(out, (size_t)out_len, format, host_ip_address_string(host), time_ms, socket_get_friendly_type_name(host->Type), host->Port);
	out[out_len - 1] = '\0';

	return SUCCESS;
}

int host_format_connect_info(const host_t *host, pc_t out, int out_len)
{
	pcc_t format;

	if (host == NULL || out == NULL || out_len <= 0) return ERROR_INVALIDARGUMENTS;

	if (host->HostIsIP)
	{
		format = i18n_get_string(STRING_CONNECT_INFO_IP);
		snprintf(out, (size_t)out_len, format, host->Hostname, socket_get_friendly_type_name(host->Type), host->Port);
	}
	else
	{
		format = i18n_get_string(STRING_CONNECT_INFO_FULL);
		snprintf(out, (size_t)out_len, format, host->Hostname, host_ip_address_string(host), socket_get_friendly_type_name(host->Type), host->Port);
	}

	out[out_len - 1] = '\0';
	return SUCCESS;
}

pcc_t host_get_ip_address_as_string(struct in_addr ip_address)
{
	return inet_ntoa(ip_address);
}

