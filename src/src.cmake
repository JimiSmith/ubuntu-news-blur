find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Concurrent REQUIRED)

set(CMAKE_AUTOMOC ON)
  # As moc files are generated in the binary dir, tell CMake
  # to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-std=c++11)

qt5_add_resources(newsblur_image_resources images/images.qrc)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_BINARY_DIR}
        ${Qt5Quick_INCLUDE_DIRS}
        ${Qt5Concurrent_INCLUDE_DIRS}
)
# We add our source code here
set(newsblur_src src/main.cpp
                 src/apihelper.cpp
                 src/networkmanager.cpp
                 src/newsblurapi.cpp
                 src/newsblurapp.cpp)

include(src/qml/qml.cmake)

# Now make sure all files get to the right place
add_executable(newsblur ${newsblur_src} ${newsblur_qml_resources} ${newsblur_image_resources})

qt5_use_modules(newsblur Quick Qml Widgets Core Xml Sql)

INSTALL(TARGETS newsblur
    RUNTIME DESTINATION bin)

install(PROGRAMS newsblur.desktop
    DESTINATION ${CMAKE_INSTALL_PREFIX}/share/applications)

