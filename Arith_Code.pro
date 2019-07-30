VERSION = "1.0.0.0"                                                                                # 软件版本号

####################################################################################################

RC_ICONS += Icons/Icon_256.ico \                                                                   # 软件图标
            Icons/Icon_128.ico \
            Icons/Icon_64.ico \
            Icons/Icon_32.ico

####################################################################################################

QMAKE_TARGET_PRODUCT = "Arith_Code"                                                                # 产品名称
QMAKE_TARGET_DESCRIPTION = "Arithmetic coding demostrator"                                         # 产品描述
QMAKE_TARGET_COMPANY = "Innogence Technology Co.,Ltd."                                             # 公司名称
QMAKE_TARGET_COPYRIGHT = "Copyright(C) 2019 Innogence Technology Inc."                             # 版权所有

####################################################################################################

QT -= gui                                                                                          # 去除GUI

DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT                                             # 添加定义（不建议继续使用的API给出警告、开启日志消息输出功能）

TEMPLATE = app                                                                                     # 以app为配置模板

TARGET = Arith_Code                                                                                # 设置目标文件名称

CONFIG += c++11 console                                                                            # 添加c++11标准和控制台
CONFIG -= app_bundle                                                                               # 去除app bundle
CONFIG -= debug_and_release                                                                        # 去除默认的debug和release同时准备编译的选项
CONFIG(debug, debug | release){ DESTDIR = debug } else { DESTDIR = release }                       # debug时只输出debug相关文件，而release时只输出release相关的文件

UI_DIR = metadata/ui                                                                               # 设置生成ui相关源文件的路径
MOC_DIR = metadata/moc                                                                             # 设置生成编译中间文件的路径
OBJECTS_DIR = metadata/objects                                                                     # 设置对象文件的路径
RCC_DIR = metadata/rcc                                                                             # 设置qrc生成中间文件的路径

INCLUDEPATH += \                                                                                   # 添加包含路径
    $$PWD/Arith_Code

HEADERS += \                                                                                       # 添加头文件
    Arith_Code/arith_code.h \
    Arith_Code/arith_decode.h \
    Arith_Code/arith_define.h \
    Arith_Code/arith_encode.h \
    Arith_Code/arith_model.h

SOURCES += \                                                                                       # 添加源文件
    Arith_Code/arith_code.c \
    Arith_Code/arith_decode.c \
    Arith_Code/arith_encode.c \
    Arith_Code/arith_model.c \
    main.c
