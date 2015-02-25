include(FindZLIB)

include(libisofs/CMakeLists.txt)

include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

set(kio_iso_PART_SRCS 
    kisodirectory.cpp 
    kisofile.cpp 
    qfilehack.cpp 
    kiso.cpp 
    iso.cpp)

add_library(kio_iso ${kio_iso_PART_SRCS} ${libisofs_SRCS})

target_link_libraries(kio_iso KF5::Archive ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} ${ZLIB_LIBRARY})

install(TARGETS kio_iso DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES iso.protocol DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES kio_isorc DESTINATION ${CONFIG_INSTALL_DIR})
