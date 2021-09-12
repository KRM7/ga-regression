QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++latest

QMAKE_CXXFLAGS += -wd4828
QMAKE_CXXFLAGS_RELEASE += -Oi -Ot -Qpar -fp:precise

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    include/regression_ga/src/fitness/converter.cpp \
    include/regression_ga/src/fitness/decoder.cpp \
    include/regression_ga/src/fitness/fitness_function.cpp \
    include/regression_ga/src/fitness/math_ops.cpp \
    include/regression_ga/src/genetic/crossover.cpp \
    include/regression_ga/src/genetic/ga.cpp \
    include/regression_ga/src/genetic/generate.cpp \
    include/regression_ga/src/genetic/mutation.cpp \
    include/regression_ga/src/io_utils.cpp \
    include/regression_ga/src/printer.cpp \
    main.cpp \
    genetic_regression.cpp

HEADERS += \
    genetic_regression.h \
    include/regression_ga/include/genetic_algorithm/base_ga.h \
    include/regression_ga/include/genetic_algorithm/binary_ga.h \
    include/regression_ga/include/genetic_algorithm/genetic_algorithm.h \
    include/regression_ga/include/genetic_algorithm/integer_ga.h \
    include/regression_ga/include/genetic_algorithm/mo_detail.h \
    include/regression_ga/include/genetic_algorithm/permutation_ga.h \
    include/regression_ga/include/genetic_algorithm/real_ga.h \
    include/regression_ga/include/genetic_algorithm/reference_points.h \
    include/regression_ga/include/genetic_algorithm/rng.h \
    include/regression_ga/src/fitness/converter.h \
    include/regression_ga/src/fitness/decoder.h \
    include/regression_ga/src/fitness/fitness_function.h \
    include/regression_ga/src/fitness/math_ops.h \
    include/regression_ga/src/fitness/token.h \
    include/regression_ga/src/genetic/crossover.h \
    include/regression_ga/src/genetic/ga.h \
    include/regression_ga/src/genetic/gene.h \
    include/regression_ga/src/genetic/generate.h \
    include/regression_ga/src/genetic/mutation.h \
    include/regression_ga/src/io_utils.h \
    include/regression_ga/src/printer.h

FORMS += \
    genetic_regression.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    include/regression_ga/LICENSE \
    include/regression_ga/include/genetic_algorithm/LICENSE
