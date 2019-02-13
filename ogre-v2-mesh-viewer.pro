
message(Qt version: $$[QT_VERSION])

QT += widgets
CONFIG -= flat
DEFINES += APP_VERSION_NUMBER=\\\"0.1.0\\\"

OGREHOME = $$(OGREHOME)
isEmpty(OGREHOME) {
    OGREHOME = "C:/SDK/OgreSDK/Ogre/build/sdk"
}

message(OGRE_HOME: $$OGREHOME)

HEADERS = \
    src/mainwindow.h \
    src/ogremanager.h \
    src/ogrewidget.h \
    src/lightwidget.h \
    src/tiny_obj_loader.h \
    src/objimporter.h \
    src/cameramanager.h \
    src/objexporter.h \
    src/scopeguard.h \
    src/batchconversiondialog.h \
    src/loadfromfolderdialog.h \
    src/OgreXML/OgreXMLMeshSerializer.h \
    src/TinyXML/tinyxml.h \
    src/TinyXML/tinystr.h \
    src/OgreGLTF/Ogre_glTF.hpp \
    src/OgreGLTF/Ogre_glTF_common.hpp \
    src/OgreGLTF/Ogre_glTF_DLL.hpp \
    src/OgreGLTF/Ogre_glTF_internal_utils.hpp \
    src/OgreGLTF/Ogre_glTF_materialLoader.hpp \
    src/OgreGLTF/Ogre_glTF_modelConverter.hpp \
    src/OgreGLTF/Ogre_glTF_OgrePlugin.hpp \
    src/OgreGLTF/Ogre_glTF_OgreResource.hpp \
    src/OgreGLTF/Ogre_glTF_skeletonImporter.hpp \
    src/OgreGLTF/Ogre_glTF_textureImporter.hpp \
    src/TinyGLTF/tiny_gltf.h \
    src/TinyGLTF/stb_image.h \
    src/TinyGLTF/stb_image_write.h \
    src/TinyGLTF/tiny_gltf.h \
    src/TinyGLTF/json.hpp \
    src/meshloader.h \
    src/scenetreewidget.h \
    src/scenetreemodel.h

SOURCES = \
    src/main.cpp \
    src/mainwindow.cpp \
    src/ogremanager.cpp \
    src/ogrewidget.cpp \ 
    src/lightwidget.cpp \
    src/objimporter.cpp \
    src/cameramanager.cpp \
    src/objexporter.cpp \
    src/batchconversiondialog.cpp \
    src/loadfromfolderdialog.cpp \
    src/OgreXML/OgreXMLMeshSerializer.cpp \
    src/TinyXML/tinyxml.cpp \
    src/TinyXML/tinystr.cpp \
    src/TinyXML/tinyxmlerror.cpp \
    src/TinyXML/tinyxmlparser.cpp \
    src/OgreGLTF/Ogre_glTF.cpp \
    src/OgreGLTF/Ogre_glTF_common.cpp \
    src/OgreGLTF/Ogre_glTF_materialLoader.cpp \
    src/OgreGLTF/Ogre_glTF_modelConverter.cpp \
    src/OgreGLTF/Ogre_glTF_OgrePlugin.cpp \
    src/OgreGLTF/Ogre_glTF_OgreResource.cpp \
    src/OgreGLTF/Ogre_glTF_skeletonImporter.cpp \
    src/OgreGLTF/Ogre_glTF_textureImporter.cpp \
    src/meshloader.cpp \
    src/scenetreewidget.cpp \
    src/scenetreemodel.cpp

FORMS += \
    src/mainwindow.ui \
    src/lightwidget.ui \
    src/batchconversiondialog.ui \
    src/loadfromfolderdialog.ui \
    src/scenetreewidget.ui

PRECOMPILED_HEADER = src/stdafx.h

INCLUDEPATH += "src"
INCLUDEPATH += "$$OGREHOME/include"
INCLUDEPATH += "$$OGREHOME/include/OGRE"
INCLUDEPATH += "$$OGREHOME/include/OGRE/Hlms/Pbs"
INCLUDEPATH += "$$OGREHOME/include/OGRE/Hlms/Unlit"
INCLUDEPATH += "src/TinyXML"
INCLUDEPATH += "src/TinyObjLoader"
INCLUDEPATH += "src/TinyGLTF"

DEFINES += TIXML_USE_STL

Debug:LIBS += -L"$$OGREHOME/lib/debug"
Debug:LIBS += -L"$$OGREHOME/lib/debug/opt"
Release:LIBS += -L"$$OGREHOME/lib/release"
Release:LIBS += -L"$$OGREHOME/lib/release/opt"

CONFIG(debug, debug|release):LIBS += \
    -lopengl32 \
    -lOgreMain_d \
    -lOgreHlmsPbs_d \
    -lOgreHlmsUnlit_d \
    -lOgreOverlay_d

CONFIG(release, debug|release):LIBS += \
    -lopengl32 \
    -lOgreMain \
    -lOgreHlmsPbs \
    -lOgreHlmsUnlit \
    -lOgreOverlay

Release:DESTDIR = ./bin
Debug:DESTDIR = ./dbin
target.path = $$[QTDIR]/
INSTALLS += target

