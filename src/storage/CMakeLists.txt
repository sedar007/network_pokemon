find_package(nlohmann_json CONFIG REQUIRED)

set(INCROOT "${PROJECT_SOURCE_DIR}/include/storage")
set(SRCROOT "${PROJECT_SOURCE_DIR}/src/storage")

set(SRC
        "${SRCROOT}/pch.h"
        "${SRCROOT}/json.cpp"
        "${SRCROOT}/storage_manager.cpp"
)

add_library(${PROJECT_NAME}-storage
        STATIC
        ${SRC}
)

target_include_directories(${PROJECT_NAME}-storage
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}-storage
        PRIVATE
        nlohmann_json::nlohmann_json
        ${PROJECT_NAME}-model
)

target_compile_options(${PROJECT_NAME}-storage
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
        $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}-storage
        PRIVATE
        "${SRCROOT}/pch.h"
)

target_compile_definitions(${PROJECT_NAME}-storage
        PRIVATE
        NETWORK_POKEMON_EXPORTS
)
