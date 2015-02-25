set(BookMan_SRCS
    krbookmark.cpp
    krbookmarkbutton.cpp
    krbookmarkhandler.cpp
    kraddbookmarkdlg.cpp)

add_library(BookMan STATIC ${BookMan_SRCS})

target_link_libraries(BookMan KF5::Wallet)
