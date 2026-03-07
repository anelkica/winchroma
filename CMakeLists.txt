cmake_minimum_required(VERSION 3.16)

project(winchroma VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2) # QuickControls2 for Windows theme

qt_standard_project_setup(REQUIRES 6.8)

qt_add_resources(RESOURCES resources.qrc) # why doesn't Qt Creator do this by itself???
qt_add_executable(appwinchroma src/main.cpp ${RESOURCES})

qt_add_qml_module(appwinchroma
    URI winchroma
    SOURCES
        src/registry/registry_manager.h src/registry/registry_manager.cpp
        src/utils/process_utils.h src/utils/process_utils.cpp
        src/dwm/window_effects.h src/dwm/window_effects.cpp
        src/dwm/window_watcher.h src/dwm/window_watcher.cpp
        src/config/config_manager.h src/config/config_manager.cpp
        src/app_settings.hpp
    QML_FILES
        qml/Main.qml
        qml/pages/BordersPage.qml
        qml/pages/TitlebarsPage.qml
        qml/pages/HighlightsPage.qml

        qml/components/ColorPanel.qml
    RESOURCES
        resources.qrc
        config.toml
        QML_FILES qml/pages/ConfigPage.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appwinchroma PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appwinchroma
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appwinchroma PRIVATE
    src
    src/dwm
    src/config
    src/registry
    src/utils
    third_party/WinReg
    third_party/tomlplusplus
)

target_link_libraries(appwinchroma PRIVATE
    Qt6::Quick
    Qt6::QuickControls2 # for Windows theme
    dwmapi
    psapi
)

include(GNUInstallDirs)
install(TARGETS appwinchroma
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
