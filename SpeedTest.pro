CONFIG += c++11
LIBS += -lpthread

INCLUDEPATH += \
    src  \
    src/TCP \

SOURCES += \
    src/main.cpp \
    src/SpeedChecking.cpp \
    src/TCP/COBData.cpp \
    src/TCP/TCPClient.cpp \
    src/TCP/TCPServer.cpp

HEADERS += \    
    src/json.hpp \
    src/SpeedChecking.h \
    src/TCP/COBData.h \
    src/TCP/TCPClient.h \
    src/TCP/TCPServer.h

DISTFILES += \
    style.astylerc
