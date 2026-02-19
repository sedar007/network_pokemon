#pragma once

namespace pokemon::tcp
{
	class poke_net::impl
	{
	public:

		impl(in_port_t port) noexcept;
	    impl() = delete;
		~impl();
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;
		impl(impl&&) noexcept = default;
		impl& operator=(impl&&) noexcept = default;
	    void listen(std::function<void(std::unique_ptr<IConnection>)> on_connect);

	private:
	    std::unique_ptr<sockpp::tcp_acceptor> m_acceptor;


	};
}
