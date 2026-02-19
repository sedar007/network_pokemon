set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(sockpp CONFIG REQUIRED)


set(INCROOT "${PROJECT_SOURCE_DIR}/include/sockpp")
set(SRCROOT "${PROJECT_SOURCE_DIR}/src/sockpp")

set(SRC
        "${SRCROOT}/pch.h"
        "${SRCROOT}/pch.cpp"
        "${SRCROOT}/tcp_acceptor.cpp"
        "${SRCROOT}/tcp_acceptor.impl.cpp"
        "${SRCROOT}/poke_net.cpp"
        "${SRCROOT}/poke_net.impl.cpp"
        "${SRCROOT}/client_net_helper.cpp"


)

add_library(${PROJECT_NAME}-sockpp
        STATIC
        ${SRC}
)

target_include_directories(${PROJECT_NAME}-sockpp
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${sockpp_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}-sockpp
        PUBLIC
            Sockpp::sockpp-static

)

target_compile_options(${PROJECT_NAME}-sockpp
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
        $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}-sockpp
        PRIVATE
        "${SRCROOT}/pch.h"
)

target_compile_definitions(${PROJECT_NAME}-sockpp
        PRIVATE
        NETWORK_POKEMON_EXPORTS
)
