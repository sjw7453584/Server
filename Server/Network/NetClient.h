//#include "INetInterface.h"
//#include "uv.h"
//
//class NetClient : public IClient
//{
//public:
//	void OnConnect(uv_stream_t* server);
//	virtual void OnConnect();
//	virtual int	 ParsePkgLen(void* pBuf, int len);
//	virtual void OnRecv(void* pBuf, int len);
//
//	virtual void OnDisconnect();
//	//void SetHandler(uv_tcp_t* handler) { client_handler = handler; }
//	virtual ~NetClient() {};
//private:
//	uv_tcp_t* client_handler;
//	static void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
//	static void on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
//};
