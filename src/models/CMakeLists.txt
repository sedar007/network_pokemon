set(INCROOT "${PROJECT_SOURCE_DIR}/include/models")
set(SRCROOT "${PROJECT_SOURCE_DIR}/src/models")

set(SRC
	"${SRCROOT}/pch.h"
    "${SRCROOT}/pch.cpp"
 )

add_library(${PROJECT_NAME}-model
	SHARED
	        ${SRC}
)

target_include_directories(${PROJECT_NAME}-model
	PUBLIC
        ${PROJECT_SOURCE_DIR}/include
		${sockpp_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}-model

)

target_compile_options(${PROJECT_NAME}-model
	PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
		$<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wall -Wextra>
		$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
		$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
		$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
		$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}-model
	PRIVATE
	        "${SRCROOT}/pch.h"
)

target_compile_definitions(${PROJECT_NAME}-model
	PRIVATE
        NETWORK_POKEMON_EXPORTS
)
