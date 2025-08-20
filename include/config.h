#pragma once

#if defined(_WIN32)
	#define NETWORK_POKEMON_API_EXPORT __declspec(dllexport)
	#define NETWORK_POKEMON_API_IMPORT __declspec(dllimport)
#else
	#define NETWORK_POKEMON_API_EXPORT __attribute__((__visibility__("default")))
	#define NETWORK_POKEMON_API_IMPORT __attribute__((__visibility__("default")))
#endif
