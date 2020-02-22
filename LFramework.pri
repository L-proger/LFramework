SOURCES += $$files($$PWD/src/*.cpp, true)
HEADERS += $$files($$PWD/src/*.h, true) \
    $$PWD/src/LFramework/USB/Device/UsbDEndpoint.h \
    $$PWD/src/LFramework/USB/Host/UsbHEndpoint.h \
    $$PWD/src/LFramework/USB/Host/UsbHException.h \
    $$PWD/src/LFramework/USB/Host/UsbHInterface.h
INCLUDEPATH += $$PWD/Src
