include(../plugin.pri)
DEFINES += NEWTON_ADVENTURE_LIBRARY
HEADERS += newton_adventure_global.h \
    newton_adventure_plugin.h \
    protobuf/newton_adventure_level.pb.h \
    protobuf/nanim.pb.h \
    newtonadventurewriter.h \
    polygontools.h
SOURCES += newton_adventure_plugin.cpp \
    protobuf/newton_adventure_level.pb.cc \
    protobuf/nanim.pb.cc \
    newtonadventurewriter.cpp \
    polygontools.cpp
OTHER_FILES += protobuf/newton_adventure_level.proto \
    protobuf/nanim.proto
LIBS += -lprotobuf
