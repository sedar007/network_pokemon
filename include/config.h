#pragma once

#if defined(_WIN32)
  #define POKEMON_API_EXPORT __declspec(dllexport)
  #define POKEMON_API_IMPORT __declspec(dllimport)
#else
  #define POKEMON_API_EXPORT __attribute__((visibility("default")))
  #define POKEMON_API_IMPORT __attribute__((visibility("default")))
#endif

#ifdef _MSC_VER
  #pragma warning(disable : 4251)
  #pragma warning(disable : 4275)
#endif
