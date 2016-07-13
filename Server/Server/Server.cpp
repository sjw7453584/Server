#include "uv.h"
#include<iostream>

void on_connection(uv_stream_t* server, int status)
{
	std::cout << "client come in";
}

int main()
{
	uv_tcp_t * server = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), server);

	sockaddr_in addr;
	uv_ip4_addr("192.168.1.122", 1122, &addr);

	uv_tcp_bind(server, (sockaddr*)&addr, 0);

	uv_listen((uv_stream_t*)server, 100, on_connection);

	return uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
