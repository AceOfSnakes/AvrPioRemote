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
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->size());
    QString compiler;
    QString tmp;
#ifdef __clang_version__
    compiler.append(QString().sprintf("Compiler: clang %s",  __clang_version__));
#elif defined __GNUC__ && defined __VERSION__
    compiler.append(QString().sprintf("Compiler: gcc %s", __VERSION__));
#elif defined _MSC_VER
    compiler.append("Compiler: Visual Studio");
#if _MSC_VER >= 1910 && _MSC_VER <= 1919
    compiler.append(" 2017 / MSVC++ 15.0");
#elif _MSC_VER >= 1920
    compiler.append(" 2019 / MSVC++ 16.0");
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
    compiler.append(" 2003 / MSVC++ 7.1");
#else
    compiler.append(", unrecognised version");
#endif
    compiler.append(QString().sprintf(" (_MSC_VER=%d)", (int)_MSC_VER));
#endif
    ui->labelCompiler->setText(compiler);
    ui->labelQT->setText(QString("Based on Qt ")
    .append(QT_VERSION_STR)
#ifdef Q_OS_WIN64
    .append(" x64")
#endif
     );
    setWindowFlags(windowFlags() & (~Qt::WindowContextHelpButtonHint));

    QImage img(":/new/prefix1/images/Built_with_Qt_RGB_logo.png");
    ui->labelQTLogo->setPixmap(QPixmap::fromImage(img));
    QObject::connect(ui->labelQTLogo, SIGNAL(customContextMenuRequested(QPoint)), qApp, SLOT(aboutQt()));
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
