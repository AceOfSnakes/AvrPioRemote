#include "comboboxdelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QDebug>
#include "wiringmodel.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    :QItemDelegate(parent)
{
}


QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex & index ) const
{
    //qDebug() << "createEditor";
    QComboBox* editor = new QComboBox(parent);
    WiringModel& model = *((WiringModel*)index.model());
    switch (index.column())
    {
    case 2:
        for(int i = 0; i < model.m_AudioList.size(); i++)
        {
            editor->addItem((model.m_AudioList.constBegin() + i).value());
        }
        break;
    case 3:
        for(int i = 0; i < model.m_VideoList.size(); i++)
        {
            editor->addItem((model.m_VideoList.constBegin() + i).value());
        }
        break;
    case 4:
        for(int i = 0; i < model.m_HDMIList.size(); i++)
        {
            editor->addItem((model.m_HDMIList.constBegin() + i).value());
        }
        break;
    }
    return editor;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //qDebug() << "setEditorData";
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    WiringModel& model = *((WiringModel*)index.model());
    //int value = index.model()->data(index, Qt::EditRole).toUInt();
    //comboBox->setCurrentIndex(value);

    switch (index.column())
    {
    case 2:
        for(int i = 0; i < model.m_AudioList.size(); i++)
        {
            if (model.m_Inputs[index.row()].audio == (model.m_AudioList.constBegin() + i).key())
            {
                comboBox->setCurrentIndex(i);
                break;
            }
        }
        break;
    case 3:
        for(int i = 0; i < model.m_VideoList.size(); i++)
        {
            if (model.m_Inputs[index.row()].audio == (model.m_VideoList.constBegin() + i).key())
            {
                comboBox->setCurrentIndex(i);
                break;
            }
        }
        break;
    case 4:
        for(int i = 0; i < model.m_HDMIList.size(); i++)
        {
            if (model.m_Inputs[index.row()].audio == (model.m_HDMIList.constBegin() + i).key())
            {
                comboBox->setCurrentIndex(i);
                break;
            }
        }
        break;
    }

}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //qDebug() << "setModelData";
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    //qDebug() << "updateEditorGeometry";
    editor->setGeometry(option.rect);
}

//void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//  QStyleOptionViewItemV4 myOption = option;
//  QString text = Items[index.row()].c_str();

//  myOption.text = text;

//  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
//}

