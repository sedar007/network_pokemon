set(SRCROOT "${PROJECT_SOURCE_DIR}/src/console")

set(SRC
        "${SRCROOT}/main.cpp"
        "${SRCROOT}/pch.h"
        "${SRCROOT}/pch.cpp"
)

add_executable(${PROJECT_NAME}-console
        ${SRC}
)

target_include_directories(${PROJECT_NAME}-console
        PUBLIC
            ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}-console
        PRIVATE
            ${PROJECT_NAME}-network-pokemon
)

target_compile_options(${PROJECT_NAME}-console
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
            $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
            $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
            $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
            $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}-console
        PRIVATE
            "${SRCROOT}/pch.h"
)
