#-------------------------------------------------
#
# Project created by QtCreator 2013-04-19T23:58:49
#
#-------------------------------------------------

QT       += core gui
QT       += network xml
QT       += opengl
unix: QT += x11extras


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT_INSTALL_PREFIX = $$[QT_INSTALL_PREFIX]
X64 = $$find(QT_INSTALL_PREFIX, 64)

TARGET = AVRPioRemote
TEMPLATE = app
win32: RC_ICONS = pio-icon1.ico

static { # everything below takes effect with CONFIG += static
    CONFIG += static
#    CONFIG += staticlib # this is needed if you create a static library, not a static executable
    DEFINES += STATIC
    message("~~~ static build ~~~") # this is for information, that the static build is done
    win32: TARGET = $$join(TARGET,,,) #this adds an s in the end, so you can seperate static build from non static build
}
SOURCES += main.cpp \
        avrpioremote.cpp \
        Defs.cpp \
    netradiodialog.cpp \
    bluraydialog.cpp \
    aboutdialog.cpp \
    loudspeakersettingsdialog.cpp \
    tunerdialog.cpp \
    actionwithparameter.cpp \
    playerinterface.cpp \
    testdialog.cpp \
    settingsdialog.cpp \
    logger.cpp \
    eqdialog.cpp \
    listeningmodedialog.cpp \
    usbdialog.cpp \
    zonecontroldialog.cpp \
    avsettingsdialog.cpp \
    mcacceqdialog.cpp \
    autosearchdialog.cpp \
    wiringdialog.cpp \
    wiringmodel.cpp \
    comboboxdelegate.cpp \
    checkboxdelegate.cpp \
    emphasisdialog.cpp \
    labeledslider.cpp \
    hdmicontroldialog.cpp \
    receiver_interface/msgdistributor.cpp \
    receiver_interface/response/receivedobjectbase.cpp \
    receiver_interface/response/responselistener.cpp \
    receiver_interface/command/cmdbase.cpp \
    receiver_interface/receiverinterface.cpp \
    receiver_interface/response/zonenumberbase.cpp \
    infodialog.cpp \
    graphiclswidget.cpp \
    receiver_interface/response/audiostatusdataresponse_ast.cpp \
    receiver_interface/response/soundretrieverresponse_ata.cpp \
    receiver_interface/response/eqresponse_atb.cpp \
    receiver_interface/response/hibitresponse_ati.cpp \
    receiver_interface/response/errorresponse_b_e.cpp \
    receiver_interface/response/bassresponse_ba_zgb.cpp \
    receiver_interface/response/channellevelresponse_clv.cpp \
    receiver_interface/response/displaydataresponse_fl.cpp \
    receiver_interface/response/inputfunctionresponse_fn.cpp \
    receiver_interface/response/emphasisresponse_ilv.cpp \
    receiver_interface/response/phasecontrolresponse_is.cpp \
    receiver_interface/response/mcaccnumberresponse_mc.cpp \
    receiver_interface/response/muteresponse_mut_z2mut_z3mut.cpp \
    receiver_interface/response/pqlscontrolresponse_pq.cpp \
    receiver_interface/response/powerresponse_pwr_apr_bpr_zep.cpp \
    receiver_interface/response/inputnameresponse_rgb.cpp \
    receiver_interface/response/speakersystemrequest_ssf.cpp \
    receiver_interface/response/speakersettingresponse_ssg.cpp \
    receiver_interface/response/surroundpositionresponse_ssp.cpp \
    receiver_interface/response/speakerdistanceresponse_sss.cpp \
    receiver_interface/response/xcurveresponse_sst.cpp \
    receiver_interface/response/hdmicontrolmoderesponse_str.cpp \
    receiver_interface/response/hdmicontrolresponse_stq.cpp \
    receiver_interface/response/hdmicontrolarcresponse_stt.cpp \
    receiver_interface/response/hdmipassthroughresponse_stu.cpp \
    receiver_interface/response/mcacceqresponse_suw.cpp \
    receiver_interface/response/toneresponse_to_zga.cpp \
    receiver_interface/response/trebleresponse_tr_zgg.cpp \
    receiver_interface/response/volumeresponse_vol_zv_yv.cpp \
    receiver_interface/response/videostatusdataresponse_vst.cpp \
    receiver_interface/response/xoverresponse_ssq.cpp \
    receiver_interface/response/response_aub.cpp \
    receiver_interface/response/speakercontrolresponse_spk.cpp \
    mcaccprogressdialog.cpp \
    receiver_interface/response/mcaccprogressresponse_ssj.cpp \
    themereader.cpp


HEADERS  += avrpioremote.h \
         Defs.h \
    netradiodialog.h \
    bluraydialog.h \
    aboutdialog.h \
    loudspeakersettingsdialog.h \
    tunerdialog.h \
    actionwithparameter.h \
    playerinterface.h \
    testdialog.h \
    settingsdialog.h \
    logger.h \
    eqdialog.h \
    listeningmodedialog.h \
    usbdialog.h \
    zonecontroldialog.h \
    avsettingsdialog.h \
    mcacceqdialog.h \
    autosearchdialog.h \
    wiringdialog.h \
    wiringmodel.h \
    comboboxdelegate.h \
    checkboxdelegate.h \
    emphasisdialog.h \
    labeledslider.h \
    hdmicontroldialog.h \
    receiver_interface/msgdistributor.h \
    receiver_interface/response/receivedobjectbase.h \
    receiver_interface/response/responselistener.h \
    receiver_interface/command/cmdbase.h \
    receiver_interface/receiverinterface.h \
    receiver_interface/response/zonenumberbase.h \
    infodialog.h \
    graphiclswidget.h \
    receiver_interface/response/audiostatusdataresponse_ast.h \
    receiver_interface/response/soundretrieverresponse_ata.h \
    receiver_interface/response/eqresponse_atb.h \
    receiver_interface/response/hibitresponse_ati.h \
    receiver_interface/response/errorresponse_b_e.h \
    receiver_interface/response/bassresponse_ba_zgb.h \
    receiver_interface/response/channellevelresponse_clv.h \
    receiver_interface/response/displaydataresponse_fl.h \
    receiver_interface/response/inputfunctionresponse_fn.h \
    receiver_interface/response/emphasisresponse_ilv.h \
    receiver_interface/response/phasecontrolresponse_is.h \
    receiver_interface/response/mcaccnumberresponse_mc.h \
    receiver_interface/response/muteresponse_mut_z2mut_z3mut.h \
    receiver_interface/response/pqlscontrolresponse_pq.h \
    receiver_interface/response/powerresponse_pwr_apr_bpr_zep.h \
    receiver_interface/response/inputnameresponse_rgb.h \
    receiver_interface/response/speakersystemrequest_ssf.h \
    receiver_interface/response/speakersettingresponse_ssg.h \
    receiver_interface/response/surroundpositionresponse_ssp.h \
    receiver_interface/response/speakerdistanceresponse_sss.h \
    receiver_interface/response/xcurveresponse_sst.h \
    receiver_interface/response/hdmicontrolresponse_stq.h \
    receiver_interface/response/hdmicontrolmoderesponse_str.h \
    receiver_interface/response/hdmicontrolarcresponse_stt.h \
    receiver_interface/response/hdmipassthroughresponse_stu.h \
    receiver_interface/response/mcacceqresponse_suw.h \
    receiver_interface/response/toneresponse_to_zga.h \
    receiver_interface/response/trebleresponse_tr_zgg.h \
    receiver_interface/response/volumeresponse_vol_zv_yv.h \
    receiver_interface/response/videostatusdataresponse_vst.h \
    receiver_interface/response/xoverresponse_ssq.h \
    receiver_interface/response/response_aub.h \
    receiver_interface/response/speakercontrolresponse_spk.h \
    mcaccprogressdialog.h \
    receiver_interface/response/mcaccprogressresponse_ssj.h \
    themereader.h



FORMS    += avrpioremote.ui \
    netradiodialog.ui \
    bluraydialog.ui \
    aboutdialog.ui \
    loudspeakersettingsdialog.ui \
    tunerdialog.ui \
    testdialog.ui \
    getoldfavoritelistdialog.ui \
    settingsdialog.ui \
    eqdialog.ui \
    listeningmodedialog.ui \
    usbdialog.ui \
    zonecontroldialog.ui \
    avsettingsdialog.ui \
    mcacceqdialog.ui \
    autosearchdialog.ui \
    wiringdialog.ui \
    emphasisdialog.ui \
    labeledslider.ui \
    hdmicontroldialog.ui \
    infodialog.ui \
    mcaccprogressdialog.ui


OTHER_FILES += \
    images/cancel.png \
    images/pen.png \
    images/ok.png \
    images/save.png \
    images/Gnome-video-x-generic.svg \
    images/Gnome-media-playback-start.svg \
    images/Gnome-folder-open.svg \
    images/Gnome-emblem-photos.svg \
    images/Gnome-audio-x-generic.svg \
    android/AndroidManifest.xml \
    android/res/layout/splash.xml \
    android/res/values/libs.xml \
    android/res/values/strings.xml \
    android/res/values-de/strings.xml \
    android/res/values-el/strings.xml \
    android/res/values-es/strings.xml \
    android/res/values-et/strings.xml \
    android/res/values-fa/strings.xml \
    android/res/values-fr/strings.xml \
    android/res/values-id/strings.xml \
    android/res/values-it/strings.xml \
    android/res/values-ja/strings.xml \
    android/res/values-ms/strings.xml \
    android/res/values-nb/strings.xml \
    android/res/values-nl/strings.xml \
    android/res/values-pl/strings.xml \
    android/res/values-pt-rBR/strings.xml \
    android/res/values-ro/strings.xml \
    android/res/values-rs/strings.xml \
    android/res/values-ru/strings.xml \
    android/res/values-zh-rCN/strings.xml \
    android/res/values-zh-rTW/strings.xml \
    android/version.xml \
    android/res/drawable/icon.png \
    android/res/drawable/logo.png \
    android/res/drawable-hdpi/icon.png \
    android/res/drawable-ldpi/icon.png \
    android/res/drawable-mdpi/icon.png \
    android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    android/src/org/kde/necessitas/origo/QtActivity.java \
    android/src/org/kde/necessitas/origo/QtApplication.java \
    pio-icon1.ico

RESOURCES += \
    avrpioremote.qrc

TRANSLATIONS = avrpioremote_en.ts \
               avrpioremote_de.ts \
               avrpioremote_ru.ts \

!isEmpty(X64) {
TARGET = AVRPioRemote64
}

CONFIG += exceptions rtti
