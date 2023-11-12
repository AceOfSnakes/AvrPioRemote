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

TARGET = AVRPioRemote
TEMPLATE = app
win32: RC_ICONS = src/AVRPioRemote.ico

*msvc* { # visual studio spec filter
    QMAKE_CXXFLAGS += -MP
}

static { # everything below takes effect with CONFIG += static
    CONFIG += static
#    CONFIG += staticlib # this is needed if you create a static library, not a static executable
    DEFINES += STATIC 
    message("~~~ static build ~~~") # this is for information, that the static build is done
    win32: TARGET = $$join(TARGET,,,) #this adds an s in the end, so you can seperate static build from non static build
}

VSCMD_VER = $$(VSCMD_VER)
VSVERSION = $$(VisualStudioVersion)

!isEmpty(VSCMD_VER) {
    message("~~~ VSCMD_VER $$(VSCMD_VER) ~~~")
    DEFINES += __VSCMD_VER=\\\"$$(VSCMD_VER)\\\"
    DEFINES += __VSVERSION=$$(VisualStudioVersion)
}

INCLUDEPATH += ./src/

SOURCES += \
    src/receiver_interface/command/cmdbase.cpp \
    src/receiver_interface/devices/discoverydevice.cpp \
    src/receiver_interface/devices/onkyoreceiver.cpp \
    src/receiver_interface/devices/pioneerreceiver.cpp \
    src/receiver_interface/response/audioscalarresponse_asc.cpp \
    src/receiver_interface/response/audiostatusdataresponse_ast.cpp \
    src/receiver_interface/response/bassresponse_ba_zgb.cpp \
    src/receiver_interface/response/channellevelresponse_clv.cpp \
    src/receiver_interface/response/drcresponse_ltn.cpp \
    src/receiver_interface/response/digital_filter_response_dgf.cpp \
    src/receiver_interface/response/displaydataresponse_fl.cpp \
    src/receiver_interface/response/emphasisresponse_ilv.cpp \
    src/receiver_interface/response/eqresponse_atb.cpp \
    src/receiver_interface/response/errorresponse_b_e.cpp \
    src/receiver_interface/response/fixedpcmresponse_fxp.cpp \
    src/receiver_interface/response/hdmicontrolarcresponse_stt.cpp \
    src/receiver_interface/response/hdmicontrolmoderesponse_str.cpp \
    src/receiver_interface/response/hdmicontrolresponse_stq.cpp \
    src/receiver_interface/response/hdmipassthroughresponse_stu.cpp \
    src/receiver_interface/response/hibitresponse_ati.cpp \
    src/receiver_interface/response/inputfunctionresponse_fn.cpp \
    src/receiver_interface/response/inputnameresponse_rgb.cpp \
    src/receiver_interface/response/loudnessresponse_ldm.cpp \
    src/receiver_interface/response/mcacceqresponse_suw.cpp \
    src/receiver_interface/response/mcaccnumberresponse_mc.cpp \
    src/receiver_interface/response/mcaccprogressresponse_ssj.cpp \
    src/receiver_interface/response/muteresponse_mut_z2mut_z3mut.cpp \
    src/receiver_interface/response/phasecontrolresponse_is.cpp \
    src/receiver_interface/response/powerresponse_pwr_apr_bpr_zep.cpp \
    src/receiver_interface/response/pqlscontrolresponse_pq.cpp \
    src/receiver_interface/response/receivedobjectbase.cpp \
    src/receiver_interface/response/response_aub.cpp \
    src/receiver_interface/response/responselistener.cpp \
    src/receiver_interface/response/soundretrieverresponse_ata.cpp \
    src/receiver_interface/response/speakercontrolresponse_spk.cpp \
    src/receiver_interface/response/speakerdistanceresponse_spd.cpp \
    src/receiver_interface/response/speakerdistanceresponse_sss.cpp \
    src/receiver_interface/response/speakerinformationresponse_spi.cpp \
    src/receiver_interface/response/speakersettingresponse_ssg.cpp \
    src/receiver_interface/response/speakersystemrequest_ssf.cpp \
    src/receiver_interface/response/surroundpositionresponse_ssp.cpp \
    src/receiver_interface/response/theaterfilterresponse_ras.cpp \
    src/receiver_interface/response/toneresponse_to_zga.cpp \
    src/receiver_interface/response/trebleresponse_tr_zgg.cpp \
    src/receiver_interface/response/upsampling_ups.cpp \
    src/receiver_interface/response/videostatusdataresponse_vst.cpp \
    src/receiver_interface/response/volumeresponse_vol_zv_yv.cpp \
    src/receiver_interface/response/xcurveresponse_sst.cpp \
    src/receiver_interface/response/xoverresponse_ssq.cpp \
    src/receiver_interface/response/zonenumberbase.cpp \
    src/receiver_interface/msgdistributor.cpp \
    src/receiver_interface/receiverinterface.cpp \
    src/aboutdialog.cpp \
    src/actionwithparameter.cpp \
    src/autosearchdialog.cpp \
    src/avrpioremote.cpp \
    src/avsettingsdialog.cpp \
    src/bluraydialog.cpp \
    src/checkboxdelegate.cpp \
    src/comboboxdelegate.cpp \
    src/Defs.cpp \
    src/emphasisdialog.cpp \
    src/eqdialog.cpp \
    src/graphiclswidget.cpp \
    src/hdmicontroldialog.cpp \
    src/infodialog.cpp \
    src/labeledslider.cpp \
    src/listeningmodedialog.cpp \
    src/logger.cpp \
    src/loudspeakersettingsdialog.cpp \
    src/loudspeakersettingsdialogonkyo.cpp \
    src/main.cpp \
    src/mcacceqdialog.cpp \
    src/mcaccprogressdialog.cpp \
    src/netradiodialog.cpp \
    src/playerinterface.cpp \
    src/settingsdialog.cpp \
    src/testdialog.cpp \
    src/themereader.cpp \
    src/tunerdialog.cpp \
    src/usbdialog.cpp \
    src/wiringdialog.cpp \
    src/wiringmodel.cpp \
    src/zonecontroldialog.cpp \
    src/netonkyodialog.cpp

HEADERS += \
    src/receiver_interface/command/cmdbase.h \
    src/receiver_interface/devices/discoverydevice.h \
    src/receiver_interface/devices/onkyoreceiver.h \
    src/receiver_interface/devices/pioneerreceiver.h \
    src/receiver_interface/response/audioscalarresponse_asc.h \
    src/receiver_interface/response/audiostatusdataresponse_ast.h \
    src/receiver_interface/response/bassresponse_ba_zgb.h \
    src/receiver_interface/response/channellevelresponse_clv.h \
    src/receiver_interface/response/drcresponse_ltn.h \
    src/receiver_interface/response/digital_filter_response_dgf.h \
    src/receiver_interface/response/displaydataresponse_fl.h \
    src/receiver_interface/response/emphasisresponse_ilv.h \
    src/receiver_interface/response/eqresponse_atb.h \
    src/receiver_interface/response/errorresponse_b_e.h \
    src/receiver_interface/response/fixedpcmresponse_fxp.h \
    src/receiver_interface/response/hdmicontrolarcresponse_stt.h \
    src/receiver_interface/response/hdmicontrolmoderesponse_str.h \
    src/receiver_interface/response/hdmicontrolresponse_stq.h \
    src/receiver_interface/response/hdmipassthroughresponse_stu.h \
    src/receiver_interface/response/hibitresponse_ati.h \
    src/receiver_interface/response/inputfunctionresponse_fn.h \
    src/receiver_interface/response/inputnameresponse_rgb.h \
    src/receiver_interface/response/loudnessresponse_ldm.h \
    src/receiver_interface/response/mcacceqresponse_suw.h \
    src/receiver_interface/response/mcaccnumberresponse_mc.h \
    src/receiver_interface/response/mcaccprogressresponse_ssj.h \
    src/receiver_interface/response/muteresponse_mut_z2mut_z3mut.h \
    src/receiver_interface/response/phasecontrolresponse_is.h \
    src/receiver_interface/response/powerresponse_pwr_apr_bpr_zep.h \
    src/receiver_interface/response/pqlscontrolresponse_pq.h \
    src/receiver_interface/response/receivedobjectbase.h \
    src/receiver_interface/response/response_aub.h \
    src/receiver_interface/response/responselistener.h \
    src/receiver_interface/response/soundretrieverresponse_ata.h \
    src/receiver_interface/response/speakercontrolresponse_spk.h \
    src/receiver_interface/response/speakerdistanceresponse_spd.h \
    src/receiver_interface/response/speakerdistanceresponse_sss.h \
    src/receiver_interface/response/speakerinformationresponse_spi.h \
    src/receiver_interface/response/speakersettingresponse_ssg.h \
    src/receiver_interface/response/speakersystemrequest_ssf.h \
    src/receiver_interface/response/surroundpositionresponse_ssp.h \
    src/receiver_interface/response/theaterfilterresponse_ras.h \
    src/receiver_interface/response/toneresponse_to_zga.h \
    src/receiver_interface/response/trebleresponse_tr_zgg.h \
    src/receiver_interface/response/upsampling_ups.h \
    src/receiver_interface/response/videostatusdataresponse_vst.h \
    src/receiver_interface/response/volumeresponse_vol_zv_yv.h \
    src/receiver_interface/response/xcurveresponse_sst.h \
    src/receiver_interface/response/xoverresponse_ssq.h \
    src/receiver_interface/response/zonenumberbase.h \
    src/receiver_interface/msgdistributor.h \
    src/receiver_interface/receiverinterface.h \
    src/aboutdialog.h \
    src/actionwithparameter.h \
    src/autosearchdialog.h \
    src/avrpioremote.h \
    src/avsettingsdialog.h \
    src/bluraydialog.h \
    src/checkboxdelegate.h \
    src/comboboxdelegate.h \
    src/Defs.h \
    src/emphasisdialog.h \
    src/eqdialog.h \
    src/graphiclswidget.h \
    src/hdmicontroldialog.h \
    src/infodialog.h \
    src/labeledslider.h \
    src/listeningmodedialog.h \
    src/logger.h \
    src/loudspeakersettingsdialog.h \
    src/loudspeakersettingsdialogonkyo.h \
    src/mcacceqdialog.h \
    src/mcaccprogressdialog.h \
    src/netradiodialog.h \
    src/playerinterface.h \
    src/settingsdialog.h \
    src/testdialog.h \
    src/themereader.h \
    src/tunerdialog.h \
    src/usbdialog.h \
    src/wiringdialog.h \
    src/wiringmodel.h \
    src/zonecontroldialog.h \
    src/netonkyodialog.h

FORMS += src/avrpioremote.ui \
    src/netradiodialog.ui \
    src/bluraydialog.ui \
    src/aboutdialog.ui \
    src/loudspeakersettingsdialog.ui \
    src/loudspeakersettingsdialogonkyo.ui \
    src/tunerdialog.ui \
    src/testdialog.ui \
    src/getoldfavoritelistdialog.ui \
    src/settingsdialog.ui \
    src/eqdialog.ui \
    src/listeningmodedialog.ui \
    src/usbdialog.ui \
    src/zonecontroldialog.ui \
    src/avsettingsdialog.ui \
    src/mcacceqdialog.ui \
    src/autosearchdialog.ui \
    src/wiringdialog.ui \
    src/emphasisdialog.ui \
    src/labeledslider.ui \
    src/hdmicontroldialog.ui \
    src/infodialog.ui \
    src/mcaccprogressdialog.ui \
    src/netonkyodialog.ui

OTHER_FILES += \
    src/images/cancel.png \
    src/images/pen.png \
    src/images/ok.png \
    src/images/save.png \
    src/images/Gnome-video-x-generic.svg \
    src/images/Gnome-media-playback-start.svg \
    src/images/Gnome-folder-open.svg \
    src/images/Gnome-emblem-photos.svg \
    src/images/Gnome-audio-x-generic.svg \
    src/android/AndroidManifest.xml \
    src/android/res/layout/splash.xml \
    src/android/res/values/libs.xml \
    src/android/res/values/strings.xml \
    src/android/res/values-de/strings.xml \
    src/android/res/values-el/strings.xml \
    src/android/res/values-es/strings.xml \
    src/android/res/values-et/strings.xml \
    src/android/res/values-fa/strings.xml \
    src/android/res/values-fr/strings.xml \
    src/android/res/values-id/strings.xml \
    src/android/res/values-it/strings.xml \
    src/android/res/values-ja/strings.xml \
    src/android/res/values-ms/strings.xml \
    src/android/res/values-nb/strings.xml \
    src/android/res/values-nl/strings.xml \
    src/android/res/values-pl/strings.xml \
    src/android/res/values-pt-rBR/strings.xml \
    src/android/res/values-ro/strings.xml \
    src/android/res/values-rs/strings.xml \
    src/android/res/values-ru/strings.xml \
    src/android/res/values-zh-rCN/strings.xml \
    src/android/res/values-zh-rTW/strings.xml \
    src/android/version.xml \
    src/android/res/drawable/icon.png \
    src/android/res/drawable/logo.png \
    src/android/res/drawable-hdpi/icon.png \
    src/android/res/drawable-ldpi/icon.png \
    src/android/res/drawable-mdpi/icon.png \
    src/android/src/org/kde/necessitas/ministro/IMinistro.aidl \
    src/android/src/org/kde/necessitas/ministro/IMinistroCallback.aidl \
    src/android/src/org/kde/necessitas/origo/QtActivity.java \
    src/android/src/org/kde/necessitas/origo/QtApplication.java \
    src/AVRPioRemote.ico

RESOURCES += \
    src/avrpioremote.qrc

TRANSLATIONS = src/avrpioremote_en.ts \
               src/avrpioremote_de.ts \
               src/avrpioremote_ru.ts \

CONFIG += exceptions rtti

DISTFILES += \
    src/images/200px-Gnome-edit-redo.svg.png
