#include "uv.h"
#include<iostream>

void on_write(uv_write_t* req, int status)
{
	if (NULL != req)
	{
		delete req;
	}
}

void OnConnect(uv_connect_t * conn, int flags)
{
	std::cout << "connet to server";
	uv_write_t *req = new uv_write_t();
	uv_buf_t buf;
	char* base = new char[100];
	strncpy(base, "sjw test", sizeof("sjw test"));
	buf = uv_buf_init(base, 100);
	uv_write(req, conn->handle, &buf, 1, on_write);
}

int main()
{
	uv_tcp_t* client = new uv_tcp_t();

	sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", 1122, &addr);
	uv_tcp_init(uv_default_loop(), client);
	uv_connect_t conn;
	uv_tcp_connect(&conn, client, (sockaddr*)&addr, OnConnect);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	return 0;
}
