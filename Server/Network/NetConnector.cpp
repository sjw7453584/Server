#include "NetConnector.h"
#include "NetInterface.h"
#include <iostream>

NetConnector::NetConnector():buffer(nullptr), m_pClient(nullptr), client_handle(nullptr)
{

}

NetConnector::~NetConnector()
{
	if (nullptr != buffer)
	{
		delete[] buffer;
	}

	if (nullptr != client_handle)
	{
		delete client_handle;
	}
}

void NetConnector::on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	auto conn = static_cast<NetConnector*>(handle->data);
	if (nullptr != conn)
	{
		if (nullptr == conn->buffer)
		{
			conn->buffer = new char[suggested_size];
		}
	}
	*buf = uv_buf_init(conn->buffer, suggested_size);
}

void NetConnector::on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	auto conn = static_cast<NetConnector*>(stream->data);
	IClient* client = nullptr;
	if (nullptr != conn)
	{
		client = conn->m_pClient;
	}

	if (nread > 0)
	{
		std::cout << buf->base;
		if (nullptr != client)
		{
			client->OnRecv(buf->base, nread);
		}
	}
	if (nread < 0)
	{
		if (nullptr != client)
		{
			client->OnDisconnect();
			((NetInterface*)INetInterface::GetInstance())->OnDisconnect(client);
		}
	}
}

void NetConnector::on_write(uv_write_t* req, int status)
{
	if (nullptr != req)
	{
		delete req;
	}
	if (0 != status)
	{
		std::cout << uv_strerror(status);
	}
}

bool NetConnector::Send(void* pBuf, int len)
{
	uv_write_t *req = new uv_write_t();
	uv_buf_t buf;
	char* base = new char[100];
	strncpy(base, "server to client\n", sizeof("server to client\n"));
	buf = uv_buf_init((char*)pBuf, len);
	uv_write(req, (uv_stream_t*)client_handle, &buf, 1, NetConnector::on_write);
}

void NetConnector::OnConnect(uv_stream_t* server)
{
	client_handle = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), client_handle);
	uv_accept(server, (uv_stream_t*)client_handle);
	client_handle->data = this;
	uv_read_start((uv_stream_t*)client_handle, NetConnector::on_alloc, NetConnector::on_read);
}