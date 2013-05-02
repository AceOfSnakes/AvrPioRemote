#include "logger.h"
#include <QDebug>


Logger* Logger::m_OwnInstance = new Logger();

Logger::Logger()
{
}

void Logger::Log(const QString& msg)
{
    //    ui->listWidgetLog->addItem(text);
    //    if (ui->listWidgetLog->count() > 100)
    //        ui->listWidgetLog->removeItemWidget(ui->listWidgetLog->item(0));
    //    ui->listWidgetLog->setCurrentItem(ui->listWidgetLog->item(ui->listWidgetLog->count() - 1));
    //    QBrush brush(color);
    //    int index = ui->listWidgetLog->currentIndex().row();
    //    ui->listWidgetLog->item(index)->setForeground(brush);
    qDebug() << msg;
}
