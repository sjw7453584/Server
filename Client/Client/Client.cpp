#include "uv.h"
#include<iostream>

void OnConnect(uv_connect_t * conn, int flags)
{
	std::cout << "connet to server";
}

int main()
{
	uv_tcp_t* client = new uv_tcp_t();

	sockaddr_in addr;
	uv_ip4_addr("192.168.1.122", 1122, &addr);

	uv_connect_t conn;
	uv_tcp_connect(&conn, client, (sockaddr*)&addr, OnConnect);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	return 0;
}
