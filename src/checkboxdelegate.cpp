#include "checkboxdelegate.h"

#include <QEvent>
#include <QModelIndex>
#include <QDebug>
#include <QApplication>
#include "wiringmodel.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QCheckBox>

CheckBoxDelegate::CheckBoxDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{
}


QWidget *CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem & option , const QModelIndex & index ) const
{
    return QStyledItemDelegate::createEditor(parent, option, index);
//    qDebug() << "createEditor";
    //QComboBox* editor = new QComboBox(parent);
//    QCheckBox* editor = new QCheckBox(parent);
//    WiringModel& model = *((WiringModel*)index.model());
//    switch (index.column())
//    {
//    case 5:

//        editor->setChecked(model.m_Inputs[index.column()].skip == "01"); // ((model.m_AudioList.constBegin() + i).value());
//        break;
//    }
//    return editor;
}

//void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    //qDebug() << "setEditorData";
//    QComboBox *comboBox = static_cast<QComboBox*>(editor);
//    WiringModel& model = *((WiringModel*)index.model());
//    //int value = index.model()->data(index, Qt::EditRole).toUInt();
//    //comboBox->setCurrentIndex(value);

//    switch (index.column())
//    {
//    case 2:
//        for(int i = 0; i < model.m_AudioList.size(); i++)
//        {
//            if (model.m_Inputs[index.row()].audio == (model.m_AudioList.constBegin() + i).key())
//            {
//                comboBox->setCurrentIndex(i);
//                break;
//            }
//        }
//        break;
//    case 3:
//        for(int i = 0; i < model.m_VideoList.size(); i++)
//        {
//            if (model.m_Inputs[index.row()].audio == (model.m_VideoList.constBegin() + i).key())
//            {
//                comboBox->setCurrentIndex(i);
//                break;
//            }
//        }
//        break;
//    case 4:
//        for(int i = 0; i < model.m_HDMIList.size(); i++)
//        {
//            if (model.m_Inputs[index.row()].audio == (model.m_HDMIList.constBegin() + i).key())
//            {
//                comboBox->setCurrentIndex(i);
//                break;
//            }
//        }
//        break;
//    }

//}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor, model, index);
//    //qDebug() << "setModelData";
//    QComboBox *comboBox = static_cast<QComboBox*>(editor);
//    model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

//void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
//{
//    //qDebug() << "updateEditorGeometry";
//    editor->setGeometry(option.rect);
//}

//void ComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//  QStyleOptionViewItemV4 myOption = option;
//  QString text = Items[index.row()].c_str();

//  myOption.text = text;

//  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
//}


void CheckBoxDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QStyleOptionViewItemV4 viewItemOption(option);
#else
    QStyleOptionViewItem viewItemOption(option);
#endif
    //if (index.column() == 0) {
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect newRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                            QSize(option.decorationSize.width() + 5,option.decorationSize.height()),
                                            QRect(option.rect.x() + textMargin, option.rect.y(),
                                                  option.rect.width() - (2 * textMargin), option.rect.height()));
        viewItemOption.rect = newRect;
   // }
    QStyledItemDelegate::paint(painter, viewItemOption, index);
}

bool CheckBoxDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                                           const QModelIndex &index)
{
    Q_ASSERT(event);
    Q_ASSERT(model);

    //qDebug() << "editorEvent";
    // make sure that the item is checkable
    Qt::ItemFlags flags = model->flags(index);
    if (!(flags & Qt::ItemIsUserCheckable) || !(flags & Qt::ItemIsEnabled))
        return false;
    // make sure that we have a check state
    QVariant value = index.data(Qt::CheckStateRole);
    if (!value.isValid())
        return false;
    // make sure that we have the right event type
    if (event->type() == QEvent::MouseButtonRelease) {
        const int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin) + 1;
        QRect checkRect = QStyle::alignedRect(option.direction, Qt::AlignCenter,
                                              option.decorationSize,
                                              QRect(option.rect.x() + (2 * textMargin), option.rect.y(),
                                                    option.rect.width() - (2 * textMargin),
                                                    option.rect.height()));
        if (!checkRect.contains(static_cast<QMouseEvent*>(event)->pos()))
            return false;
    } else if (event->type() == QEvent::KeyPress) {
        if (static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space&& static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
            return false;
    } else {
        return false;
    }
    Qt::CheckState state = (static_cast<Qt::CheckState>(value.toInt()) == Qt::Checked
                            ? Qt::Unchecked : Qt::Checked);
    //qDebug() << " ----> " + state;
    return model->setData(index, state, Qt::CheckStateRole);
}
