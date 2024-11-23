/*
 * AVRPioRemote
 * Copyright (C) 2013  Andreas Müller, Ulrich Mensfeld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "aboutdialog.h"
#include "QDateTime"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    QString compiler;
#ifdef __clang_version__
    compiler.append(QString().asprintf("Compiler: clang %s",  __clang_version__));
#elif defined __GNUC__ && defined __VERSION__
    compiler.append(QString().asprintf("Compiler: gcc %s", __VERSION__));
#elif defined _MSC_VER
    compiler.append("Compiler: Visual Studio");
#if 0    
    /*
     * https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros
     */
#elif defined __VSCMD_VER
    /*
     * Special for Qt
     * add this in .pro file
     *
     * VSCMD_VER = $$(VSCMD_VER)
     * VSVERSION = $$(VisualStudioVersion)
     *
     * !isEmpty(VSCMD_VER) {
     *    message("~~~ VSCMD_VER $$(VSCMD_VER) ~~~")
     *    DEFINES += __VSCMD_VER=\\\"$$(VSCMD_VER)\\\"
     *    DEFINES += __VSVERSION=$$(VisualStudioVersion)
     * }
     */
    compiler.append(QString().asprintf(" %d / MSVC++ %s", 2013 + (((int)__VSVERSION)-13) * 2
                                       + (((int)__VSVERSION) > 16 ? 1 : 0)
                                       , __VSCMD_VER));
#elif _MSC_VER >= 1930
    compiler.append(" 2022 / MSVC++ 17.").append(QString().asprintf("%d",((_MSC_VER % 100) - 30)));
#elif _MSC_VER >= 1929
#if _MSC_FULL_VER >= 192930100
    compiler.append(" 2019 / MSVC++ 16.11");
#else
    compiler.append(" 2019 / MSVC++ 16.10");
#endif
#elif _MSC_VER >= 1928
#if _MSC_FULL_VER >= 192829500
    compiler.append(" 2019 / MSVC++ 16.9");
#else
    compiler.append(" 2019 / MSVC++ 16.8");
#endif
#if _MSC_VER >= 1920
    compiler.append(" 2019 / MSVC++ 16.").append(QString().asprintf("%d",((_MSC_VER % 100) - 20)));
#elif _MSC_VER > 1911
    compiler.append(" 2017 / MSVC++ 15.").append(QString().asprintf("%d",((_MSC_VER % 100) - 7)));
#elif _MSC_VER == 1911
    compiler.append(" 2017 / MSVC++ 15.3");
#elif _MSC_VER == 1910
    compiler.append(" 2017 / MSVC++ 15.0");
#elif _MSC_VER == 1900
    compiler.append(" 2015 / MSVC++ 14.0");
#elif _MSC_VER == 1800
    compiler.append(" 2013 / MSVC++ 12.0");
#elif _MSC_VER == 1700
    compiler.append(" 2012 / MSVC++ 11.0");
#elif _MSC_VER == 1600
    compiler.append(" 2010 / MSVC++ 10.0");
#elif  _MSC_VER == 1500
    compiler.append(" 2008 / MSVC++ 9.0");
#elif  _MSC_VER == 1400
    compiler.append(" 2005 / MSVC++ 8.0");
#elif  _MSC_VER == 1310
    compiler.append(" .NET 2003 / MSVC++ 7.1");
#elif  _MSC_VER == 1300
    compiler.append(" .NET 2002 / MSVC++ 7.0");
#elif  _MSC_VER == 1300
    compiler.append(" .NET 2002 / MSVC++ 7.0");
#endif
#else
    compiler.append(", unrecognised version");
#endif
    compiler.append(QString().asprintf(" (_MSC_VER=%d)", (int)_MSC_VER));
#endif
    QLocale::setDefault(QLocale::English);
    QLocale myLoc;
    QDateTime date = myLoc.toDateTime(QString(__DATE__).replace("  "," ").trimmed(),"MMM d yyyy");
    QDate  first;
    first.setDate(date.date().year(),date.date().month(),1);
    int week = date.date().weekNumber() -
            (first.weekNumber() > date.date().weekNumber() ? 0 : first.weekNumber());
    ui->label_3->setText(QString("Version ").append(
#ifdef __FORCED_APP_VER
        QString(__FORCED_APP_VER)
#else
        date.toString("yy.MM")
                              .append(week == 0 ? QString() : QString().asprintf(".%d", week))
#endif
        ));
    ui->labelCompiler->setText(compiler);
    ui->labelQT->setText(QString("Based on Qt ")
    .append(qVersion())

#ifdef STATIC                                   
    .append(" (static)" )
#endif
#ifdef Q_OS_WIN64
    .append(" x64")
#endif
     );
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));

    QImage img(":/new/prefix1/images/Built_with_Qt_RGB_logo.png");
    ui->labelQTLogo->setPixmap(QPixmap::fromImage(img));
    QObject::connect(ui->labelQTLogo, SIGNAL(customContextMenuRequested(QPoint)), qApp, SLOT(aboutQt()));

    ui-> program->setText(qApp->applicationName());
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    this->close();
}


void AboutDialog::on_labelQTLogo_customContextMenuRequested(const QPoint &pos)
{

}
