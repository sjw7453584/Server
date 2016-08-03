#include "uv.h"
#include<iostream>
#include "INetInterface.h"
#include "ClientMgr.h"
#include "SqlInterface.h"
#include <locale>
#include <codecvt>
#include "SelectRole.h"
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
	if (nread < 0)
	{
		std::cout << uv_strerror(nread) << "\n";
	}
}

void on_write(uv_write_t* req, int status)
{
	if (NULL != req)
	{
		delete req;
	}
}

void on_timer(uv_timer_t* handle)
{
	uv_write_t *req = new uv_write_t();
	uv_buf_t buf;
	char* base = new char[100];
	strncpy(base, "server to client\n", sizeof("server to client\n"));
	buf = uv_buf_init(base, 100);
	uv_write(req, (uv_stream_t*)handle->data, &buf, 1, on_write);

}

void on_disconnect(uv_poll_t* handle, int status, int events)
{

}



void on_connection(uv_stream_t* server, int status)
{
	std::cout << "client come in\n";
	if (-1 == status)
	{
		std::cout << uv_strerror(status)<<"\n";
		return;
	}

	uv_tcp_t* client = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), client);
	uv_accept(server, (uv_stream_t*)client);
	uv_read_start((uv_stream_t*)client, on_alloc, on_read);

	uv_write_t *req = new uv_write_t();
	uv_buf_t buf;
	char* base = new char[100];
	strncpy(base, "server to client\n", sizeof("server to client\n"));
	buf = uv_buf_init(base, 100);
	uv_write(req, (uv_stream_t*)client, &buf, 1, on_write);


	uv_timer_t* timer = new uv_timer_t();
	timer->data = client;
	uv_timer_init(uv_default_loop(), timer);
	uv_timer_start(timer, on_timer, 1000, 1000);
	/*uv_poll_t poll_handle;
	uv_poll_init(uv_default_loop(), &poll_handle, client->socket);
	uv_poll_start(&poll_handle, UV_WRITABLE, on_disconnect);*/
}

int main()
{
	/*uv_tcp_t * server = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), server);

	sockaddr_in addr;
	uv_ip4_addr("127.0.0.1", 1122, &addr);

	uv_tcp_bind(server, (sockaddr*)&addr, 0);

	uv_listen((uv_stream_t*)server, 100, on_connection);

	return uv_run(uv_default_loop(), UV_RUN_DEFAULT);*/

	/*DataBase base;
	base.Init();*/
	/*std::locale loc("chs");
	std::cout << loc.name() << std::endl;*/
	//return 0;
	/*IpPort ip_port;
	strncpy(ip_port.szIp, "0.0.0.0", strlen("0.0.0.0"));
	ip_port.port = 1122;

	INetInterface::GetInstance()->StartListen(ip_port, ClientMgr::GetSingletonPtr());*/
	
	//std::string mystring("\xe4\xb8\xad\xe6\x96\x87");////UTF-8编码的“中文”字符串
 //   std::wstring_convert<std::codecvt_utf8<wchar_t> >cvt_utf8;//UTF-8<->Unicode转换器
	//std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>>cvt_ansi(new std::codecvt<wchar_t, char, std::mbstate_t>("CHS"));//GBK<->Unicode转换器
	//std::wstring ws = cvt_utf8.from_bytes(mystring);//UTF-8转换为Unicode
	//std::string utf_str = cvt_utf8.to_bytes(ws);
	//std::string myansistr = cvt_ansi.to_bytes(ws);//Unicode转换为GBK
	//std::cout << myansistr << std::endl;
	std::vector<connection_info> conns;
	connection_info conn;
	conn.ip = "192.168.1.72";
	conn.port = 3306;
	conn.user_name = "root";
	conn.passwd = "Sanguo1!";
	conn.index = 1;
	conn.database_name = "zgame3";
	conn.conn_codec = "latin1";
	conn.thread_num = 2;
	conns.push_back(conn);
	conn.index = 1;
	conn.database_name = "zgame4";
	conns.push_back(conn);
	SqlInterface::GetSingleton().Init(conns);
	for (auto i = 0; i < 1000; i++)
	{
		getSelectRole(1, 1);
	}
	SqlInterface::GetSingleton().Run();
	//SqlInterface::GetSingleton().JoinDbThreads();
}
