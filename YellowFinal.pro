TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    condition_parser.cpp \
    condition_parser_test.cpp \
    database.cpp \
    database_test.cpp \
    date.cpp \
    node.cpp \
    test_runner.cpp \
    token.cpp

HEADERS += \
    condition_parser.h \
    database.h \
    date.h \
    node.h \
    test_runner.h \
    token.h
