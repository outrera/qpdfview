include(qpdfview.pri)

TARGET = qpdfview_pdf
TEMPLATE = lib
CONFIG += plugin
static_pdf_plugin:CONFIG += static

TARGET_SHORT = qpdfpdf
!isEmpty(PLUGIN_DESTDIR): DESTDIR = $$PLUGIN_DESTDIR

OBJECTS_DIR = objects-pdf
MOC_DIR = moc-pdf

HEADERS = sources/global.h sources/model.h sources/pdfmodel.h sources/annotationdialog.h sources/formfielddialog.h
SOURCES = sources/pdfmodel.cpp sources/annotationdialog.cpp sources/formfielddialog.cpp

QT += core xml gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

!without_pkgconfig {
    CONFIG += link_pkgconfig
    PKGCONFIG += poppler-qt4

    system(pkg-config --atleast-version=0.14 poppler-qt4):DEFINES += HAS_POPPLER_14
    system(pkg-config --atleast-version=0.18 poppler-qt4):DEFINES += HAS_POPPLER_18
    system(pkg-config --atleast-version=0.20.1 poppler-qt4):DEFINES += HAS_POPPLER_20
    system(pkg-config --atleast-version=0.22 poppler-qt4):DEFINES += HAS_POPPLER_22
    system(pkg-config --atleast-version=0.24 poppler-qt4):DEFINES += HAS_POPPLER_24
} else {
    DEFINES += $$PDF_PLUGIN_DEFINES
    INCLUDEPATH += $$PDF_PLUGIN_INCLUDEPATH
    LIBS += $$PDF_PLUGIN_LIBS
}

!static_pdf_plugin {
    target.path = $${PLUGIN_INSTALL_PATH}
    INSTALLS += target
}
