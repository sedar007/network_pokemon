find_package(Qt6 REQUIRED COMPONENTS Quick )
find_package(sockpp CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)


set(INCROOT "${PROJECT_SOURCE_DIR}/include/networkPokemon")
set(SRCROOT "${PROJECT_SOURCE_DIR}/Ui/NetworkPokemon")
set(SRCPOKEMON "${PROJECT_SOURCE_DIR}/src/networkPokemon")


set(SRC
        "${SRCROOT}/pch.h"
        "${SRCROOT}/TestWrapper.h"
        "${SRCROOT}/src/main.cpp"
        "${SRCPOKEMON}/test.cpp"
)

qt_add_executable(${PROJECT_NAME}-UI
        ${SRC}
)

qt_add_qml_module(${PROJECT_NAME}-UI
       URI NetworkPokemon
       VERSION 1.0
       QML_FILES
       Main.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${PROJECT_NAME}-UI PROPERTIES
        #    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appNetworkPokemon
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
)

if(IOS)
    set_target_properties(${PROJECT_NAME}-UI PROPERTIES
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/ios/Info.plist"
            # RESOURCE est uniquement utilisé par Xcode lors du build,
            # NE PAS le mettre dans install()
            RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets"
    )
endif()

target_include_directories(${PROJECT_NAME}-UI
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}-UI
        PRIVATE
           Sockpp::sockpp-static
           Qt6::Quick
)

target_compile_options(${PROJECT_NAME}-UI
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS}>
        $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_DEBUG_OPTIMISATION}>
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU>>:${GCC_DEBUG_OPTIMISATION}>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:${MSVC_RELEASE_OPTIMISATION}>
        $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU>>:${GCC_RELEASE_OPTIMISATION}>
)

target_precompile_headers(${PROJECT_NAME}-UI
        PRIVATE
        "${SRCROOT}/pch.h"
)

target_compile_definitions(${PROJECT_NAME}-UI
        PRIVATE
        NETWORK_POKEMON_CLASS_EXPORTS
)

