find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

set(APP_NAME "PokeNet")
set(APP_VERSION_MAJOR "1")
set(APP_VERSION_MINOR "0")
set(APP_VERSION_PATCH "0")
set(APP_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_PATCH}")

qt_add_executable(${PROJECT_NAME}-ui
        main.cpp
)

qt_add_qml_module(${PROJECT_NAME}-ui
        URI NetworkPokemonUi
        QML_FILES
        Main.qml
        AppHeader.qml
        NavBar.qml
        pages/GalleryPage.qml
        pages/ProfilePage.qml
        pages/PeersPage.qml
        pages/SearchPage.qml
        components/PokemonDelegate.qml
        components/StatCard.qml
        components/ActionButton.qml
        components/ConfigInput.qml
        components/ConfigSwitch.qml
        components/SettingsPopup.qml
        Data/PokemonModel.qml
        SOURCES
        Backend.h Backend.cpp
        node.h node.cpp
        peermodel.h peermodel.cpp
        pokemonmodel.h pokemonmodel.cpp
)

target_include_directories(${PROJECT_NAME}-ui
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}-ui
        PRIVATE Qt6::Quick Qt6::Core Qt6::Network NetworkPokemon-network-pokemon
)


if(APPLE)
    set_target_properties(${PROJECT_NAME}-ui PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_BUNDLE_NAME "${APP_NAME}"
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.adjysedar.networkpokemon"
            MACOSX_BUNDLE_BUNDLE_VERSION "${APP_VERSION}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
    )

    if(IOS)
        set(ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets")

        set_source_files_properties("${ASSETS_PATH}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
        target_sources(${PROJECT_NAME}-ui PRIVATE "${ASSETS_PATH}")

        set_target_properties(${PROJECT_NAME}-ui PROPERTIES
                MACOSX_BUNDLE_ICON_FILE "AppIcon"
                XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
        )
    endif()
endif()

# Configuration Windows
set_target_properties(${PROJECT_NAME}-ui PROPERTIES WIN32_EXECUTABLE TRUE)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}-ui
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
