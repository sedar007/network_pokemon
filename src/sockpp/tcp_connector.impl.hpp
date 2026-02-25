#pragma once
#include <sockpp/tcp_connector.h>

namespace pokemon::tcp
{
	class tcp_connector::impl
	{
	public:
	    impl() noexcept;
		~impl();
		impl(const impl&) = delete;
		impl& operator=(const impl&) = delete;
		impl(impl&&) noexcept = default;
		impl& operator=(impl&&) noexcept = default;

	   [[nodiscard]] bool connect(std::string_view ip, int port) const noexcept;

	    bool write(std::string_view data) const noexcept ;
		bool read(char* buffer, size_t length) const noexcept ;
		void shutdown() const noexcept;

	private:
	    std::unique_ptr<sockpp::tcp_connector> m_connector;


	};
}
