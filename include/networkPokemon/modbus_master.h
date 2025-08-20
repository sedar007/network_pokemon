#pragma once

namespace pokemon
{

	class NETWORK_POKEMON_CLASS_API modbus_master
	{
	public:

		modbus_master() noexcept = default;
		~modbus_master() = default;

	public:

		bool load_from_file();

	};

}
