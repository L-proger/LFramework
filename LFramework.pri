SOURCES += $$files($$PWD/src/*.cpp, true)
HEADERS += $$files($$PWD/src/*.h, true) \
    $$PWD/Src/LFramework/COM/IUnknown.h \
    $$PWD/src/LFramework/TypeTraits/FunctionTraits.h
INCLUDEPATH += $$PWD/Src
