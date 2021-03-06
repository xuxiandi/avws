//
// web_socket.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2013 Jack (jack.wgm at gmail dot com)
//
// Distributed under the GNU Affero General Public License, Version 3.0. (See accompanying
// file LICENSE or copy at http://www.gnu.org/licenses/agpl-3.0.txt)
//

#ifndef AVWS_WEB_SOCKET_HPP
#define AVWS_WEB_SOCKET_HPP

#include "avws.hpp"

namespace avws {

using boost::asio::ip::tcp;

// 这是一个web_socket接口实现.
class web_socket : public boost::noncopyable
{
public:
	// Constructor.
	AVWS_DECL explicit web_socket(boost::asio::io_service &io);

	// Destructor.
	AVWS_DECL virtual ~web_socket();

	// 设置请求选项信息, 必须在open或accept之前设置.
	AVWS_DECL void request_options(const request_opts &options);
	// 返回当前连接的请求选项.
	AVWS_DECL request_opts request_options(void) const;
	// 返回当前连接的回复选项信息.
	AVWS_DECL response_opts response_options(void) const;

	// 在open或accept的步骤里, 实行了websocket认证.

	// 打开url.
	AVWS_DECL void open(const url &u);

	AVWS_DECL void open(const url &u, boost::system::error_code &ec);

	// 异步打开url.
	template <typename Handler>
	void async_open(const url &u, BOOST_ASIO_MOVE_ARG(Handler) handler);

	// 关闭底层连接.
	AVWS_DECL void close();
	AVWS_DECL void close(boost::system::error_code &ec);

	// 在websocket中, 数据封装成frame由发送函数实现, 上层用户无需操心.
	// 至于是文本还是二进制, 由request_options确定.
	// 由于websocket是基于message设计, 所以, write_some每次实际上将
	// 是发送一条完整的消息.

	// 同步发送消息.
	template <typename ConstBufferSequence>
	std::size_t write_some(const ConstBufferSequence &buffers);
	// 同步发送消息.
	template <typename ConstBufferSequence>
	std::size_t write_some(const ConstBufferSequence &buffers,
		boost::system::error_code &ec);

	// 异步发送消息.
	template <typename ConstBufferSequence, typename Handler>
	void async_write_some(const ConstBufferSequence &buffers, BOOST_ASIO_MOVE_ARG(Handler) handler);

	// 同步接收消息.
	template <typename MutableBufferSequence>
	std::size_t read_some(const MutableBufferSequence &buffers);
	// 同步接收消息.
	template <typename MutableBufferSequence>
	std::size_t read_some(const MutableBufferSequence &buffers,
		boost::system::error_code &ec);

	// 异步接收消息.
	template <typename MutableBufferSequence, typename Handler>
	void async_read_some(const MutableBufferSequence &buffers, BOOST_ASIO_MOVE_ARG(Handler) handler);

protected:

	// io_service引用, 由用户在构造时传入, 并由用户运行.
	boost::asio::io_service& m_io_service;

	// 解析HOST.
	tcp::resolver m_resolver;

	// 当前socket对象.
	tcp::socket m_socket;

	// 当前url.
	url m_url;

	// 当前协议.
	std::string m_protocol;
};

} // namespace avws

#include "avws/impl/web_socket.ipp"

#endif // AVWS_WEB_SOCKET_HPP
