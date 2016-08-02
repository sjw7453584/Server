//#include "NetClient.h"
//#include <iostream>
//#include "NetInterface.h"
//void NetClient::on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
//{
//	char* base = new char[10000];
//	*buf = uv_buf_init(base, 10000);
//}
//
//void NetClient::on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
//{
//	if (nread > 0)
//	{
//		std::cout << buf->base;
//	}
//	if (nread < 0)
//	{
//
//	}
//}

//void NetClient::OnConnect(uv_stream_t* server)
//{
//	client_handler = new uv_tcp_t();
//	uv_tcp_init(uv_default_loop(), client_handler);
//	uv_accept(server, (uv_stream_t*)client_handler);
//	uv_read_start((uv_stream_t*)client_handler, NetInterface::on_alloc, NetInterface::on_read);
//}
