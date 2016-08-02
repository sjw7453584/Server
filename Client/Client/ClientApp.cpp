#include "uv.h"
#include<iostream>
#include "INetInterface.h"
#include "ClientMgr.h"

//void on_close(uv_shutdown_t* req, int status)
//{
//	std::cout << "closed";
//	delete req;
//}
//
//void on_write(uv_write_t* req, int status)
//{
//	/*uv_shutdown_t* shut_req = new uv_shutdown_t();
//	uv_shutdown(shut_req, req->handle, on_close);*/
//	//uv_close((uv_handle_t*)req->handle, NULL);
//	if (NULL != req)
//	{
//		delete req;
//	}
//	if (0 != status)
//	{
//		std::cout << uv_err_name(status) << " : "<<uv_strerror(status)<<"\n";
//	}
//
//	
//}
//
//
//void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
//{
//	char* base = new char[suggested_size];
//	*buf = uv_buf_init(base, suggested_size);
//}
//
//
//
//void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
//{
//	if (nread > 0)
//	{
//		std::cout << buf->base;
//	}
//
//
//	//uv_read_stop(stream);
//	//delete[] buf->base;
//	//uv_close((uv_handle_t*)stream, NULL);
//}
//
//void on_timer(uv_timer_t* handle)
//{
//	uv_write_t *req = new uv_write_t();
//	uv_buf_t buf;
//	char* base = new char[100];
//	strncpy(base, "client to server\n", sizeof("client to server\n"));
//	buf = uv_buf_init(base, 100);
//	uv_write(req, (uv_stream_t*)handle->data, &buf, 1, on_write);
//	
//}
//
//void OnConnect(uv_connect_t * conn, int flags)
//{
//	if (UV_ECONNREFUSED == flags)
//	{
//		std::cout << uv_err_name(flags)<<"\n";
//	}
//	std::cout << "connet to server";
//
//
//	uv_write_t *req = new uv_write_t();
//	uv_buf_t buf;
//	char* base = new char[100];
//	strncpy(base, "client to server\n", sizeof("client to server\n"));
//	buf = uv_buf_init(base, 100);
//	uv_write(req, conn->handle, &buf, 1, on_write);
//	delete[] base;
//	uv_read_start((uv_stream_t*)conn->handle, on_alloc, on_read);
//
//
//	/*uv_timer_t* timer = new uv_timer_t();
//	timer->data = conn->handle;
//	uv_timer_init(uv_default_loop(), timer);
//	uv_timer_start(timer, on_timer, 1000, 1000);*/
//}

int main()
{
	//uv_tcp_t* client = new uv_tcp_t();

	//sockaddr_in addr;
	//uv_ip4_addr("127.0.0.1", 1122, &addr);
	//uv_tcp_init(uv_default_loop(), client);
	//uv_connect_t* conn = new uv_connect_t();
	//uv_tcp_connect(conn, client, (sockaddr*)&addr, OnConnect);
	//uv_run(uv_default_loop(), UV_RUN_DEFAULT);
	IpPort ip_port;
	strcpy(ip_port.szIp, "127.0.0.1");
	ip_port.port = 1122;
	auto pClient = INetInterface::GetInstance()->ConnectTo(ip_port, ClientMgr::GetSingletonPtr());
	if (NULL != pClient)
	{
		static_cast<Client*>(pClient)->Send("send test", strlen("send test")+ 1);
	}
	return 0;
}
