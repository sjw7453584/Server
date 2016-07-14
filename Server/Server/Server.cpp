#include "uv.h"
#include<iostream>

void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	char* base = new char[10000];
	*buf = uv_buf_init(base, 10000);
}

void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	if (nread > 0)
	{
		std::cout << buf->base;
	}
}

void on_connection(uv_stream_t* server, int status)
{
	std::cout << "client come in";
	if (-1 == status)
	{
		return;
	}

	uv_tcp_t* client = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), client);
	uv_accept(server, (uv_stream_t*)client);
	uv_read_start((uv_stream_t*)client, on_alloc, on_read);
}

int main()
{
	uv_tcp_t * server = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), server);

	sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", 1122, &addr);

	uv_tcp_bind(server, (sockaddr*)&addr, 0);

	uv_listen((uv_stream_t*)server, 100, on_connection);

	return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
