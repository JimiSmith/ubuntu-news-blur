set(newsblur_qml src/qml/main.qml)

add_custom_target(QmlFiles ALL ${CMAKE_COMMAND}
                  SOURCES ${newsblur_qml})

qt5_add_resources(newsblur_qml_resources src/qml/qml.qrc)
