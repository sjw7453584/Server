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
			if (nullptr != conn)
			{
				conn->Close();
			}
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

void NetConnector::OnClose(uv_handle_t* handle)
{
	if (nullptr != handle)
	{
		delete handle;
	}
}

void NetConnector::Close()
{
	if (nullptr != client_handle)
	{
		uv_close((uv_handle_t*)client_handle, NetConnector::OnClose);
	}
}

bool NetConnector::Send(void* pBuf, int len)
{
	uv_write_t *req = new uv_write_t();
	uv_buf_t buf;
	buf = uv_buf_init((char*)pBuf, len);
	uv_write(req, (uv_stream_t*)client_handle, &buf, 1, NetConnector::on_write);
	return true;
}

void NetConnector::OnConnect(uv_stream_t* server)
{
	client_handle = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), client_handle);
	uv_accept(server, (uv_stream_t*)client_handle);
	client_handle->data = this;
	uv_read_start((uv_stream_t*)client_handle, NetConnector::on_alloc, NetConnector::on_read);
}

void NetConnector::OnConnect(uv_connect_t * conn, int flags)
{
	if (UV_ECONNREFUSED == flags)
	{
		std::cout << uv_err_name(flags) << "\n";
	}
	std::cout << "connet to server";
	//uv_read_start((uv_stream_t*)conn->handle, NetConnector::on_alloc, NetConnector::on_read);
	if (NULL != conn->data )
	{
		auto pNetConn = static_cast<NetConnector*>(conn->data);
		if (nullptr != pNetConn->m_pClient)
		{
			pNetConn->m_pClient->OnConnect();
		}
	}
	if (nullptr != conn)
	{
		delete conn;
	}
}

void NetConnector::ConnectTo(const IpPort & ipPort)
{
	sockaddr_in addr;
	uv_ip4_addr(ipPort.szIp, ipPort.port, &addr);
	client_handle = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), client_handle);
	client_handle->data = this;
	uv_connect_t* conn =  new uv_connect_t();
	uv_tcp_connect(conn, client_handle, (sockaddr*)&addr, NetConnector::OnConnect);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}