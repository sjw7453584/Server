#include "INetInterface.h"
#include "uv.h"
class NetConnector : public IConnect
{
public:
	NetConnector();
	~NetConnector();
	virtual bool Send(void* pBuf, int len);
	virtual void Close();
	virtual void GetClientIpPort(unsigned int& ip, unsigned short& port) {}
	void OnConnect(uv_stream_t* server);
	void ConnectTo(const IpPort & ipPort);
	void SetClient(IClient* client) { m_pClient = client; };
private:
	uv_tcp_t* client_handle;
	IClient* m_pClient;
	char* buffer;
	static void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void on_write(uv_write_t* req, int status);
	static void OnClose(uv_handle_t* handle);
	static void OnConnect(uv_connect_t * conn, int flags);
};
