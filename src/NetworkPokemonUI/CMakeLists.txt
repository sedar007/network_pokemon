find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(${PROJECT_NAME}-ui
    main.cpp
)

if(IOS)
    set(ASSETS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/ios/Assets.xcassets")

    set_source_files_properties(
            "${ASSETS_PATH}"
            PROPERTIES
            MACOSX_PACKAGE_LOCATION Resources
    )

    target_sources(${PROJECT_NAME}-ui PRIVATE "${ASSETS_PATH}")

    set_target_properties(${PROJECT_NAME}-ui PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.adjysedar.networkpokemon"
        MACOSX_BUNDLE_BUNDLE_NAME "Network Pokémon"
        MACOSX_BUNDLE_ICON_FILE "AppIcon" # Le nom du dossier dans .appiconset
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
    )
endif()

qt_add_qml_module(${PROJECT_NAME}-ui
    URI NetworkPokemonUi
    QML_FILES
        Main.qml
        SOURCES Backend.h
        SOURCES Backend.cpp
        QML_FILES AppHeader.qml
        QML_FILES NavBar.qml
        QML_FILES
        QML_FILES pages/GalleryPage.qml
        QML_FILES pages/ProfilePage.qml
        QML_FILES components/PokemonDelegate.qml
        QML_FILES components/StatCard.qml
        QML_FILES components/ActionButton.qml
        QML_FILES Data/PokemonModel.qml
        QML_FILES pages/PeersPage.qml
        QML_FILES pages/SearchPage.qml
        SOURCES peermodel.h peermodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(${PROJECT_NAME}-ui PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_include_directories(${PROJECT_NAME}-ui
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}-ui
    PRIVATE Qt6::Quick Qt6::Core Qt6::Network NetworkPokemon-network-pokemon
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}-ui
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
