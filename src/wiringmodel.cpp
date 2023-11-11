#include "wiringmodel.h"
#include <QDebug>
#include <QFont>
#include <QBrush>

WiringModel::WiringModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    m_AudioList.insert("00", " "); //No Assign
    m_AudioList.insert("01", "COAX 1");
    m_AudioList.insert("02", "COAX 2");
    m_AudioList.insert("03", "COAX 3");
    m_AudioList.insert("04", "OPT 1");
    m_AudioList.insert("05", "OPT 2");
    m_AudioList.insert("06", "OPT 3");
    m_AudioList.insert("10", "ANALOG");

    m_HDMIList.insert("00", " "); //No Assign
    m_HDMIList.insert("01", "HDMI 1");
    m_HDMIList.insert("02", "HDMI 2");
    m_HDMIList.insert("03", "HDMI 3");
    m_HDMIList.insert("04", "HDMI 4");
    m_HDMIList.insert("05", "HDMI 5");
    m_HDMIList.insert("06", "HDMI 6");
    m_HDMIList.insert("07", "HDMI 7");

    m_VideoList.insert("00", " "); //No Assign
    m_VideoList.insert("01", "COMPONENT 1");
    m_VideoList.insert("02", "CONPONENT 2");
    m_VideoList.insert("03", "COMPONENT 3");
}

int WiringModel::rowCount(const QModelIndex &parent) const
{
//    qDebug() << "Rows " << m_Inputs.size();
    return m_Inputs.size();
}

int WiringModel::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant WiringModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    // generate a log message when this method gets called
//    qDebug() << QString("row %1, col%2, role %3")
//            .arg(row).arg(col).arg(role);

    switch(role){
    case Qt::DisplayRole:
        switch (col)
        {
        case 0:
            return m_Inputs[row].name;
        case 1:
            return m_Inputs[row].alias;
        case 2:
            return m_AudioList[m_Inputs[row].audio];
        case 3:
            return m_VideoList[m_Inputs[row].video];
        case 4:
            return m_HDMIList[m_Inputs[row].hdmi];
        //case 5:
        //    return m_Inputs[row].skip;
        }

        break;
    case Qt::ForegroundRole:
        if (m_Inputs[row].skip == "01")
        {
            QBrush brush(Qt::lightGray);
            return brush;
        }
        break;
//    case Qt::FontRole:
//        if (row == 0 && col == 0) //change font only for cell(0,0)
//        {
//            QFont boldFont;
//            boldFont.setBold(true);
//            return boldFont;
//        }
//        break;
//    case Qt::BackgroundRole:

//        if (row == 1 && col == 2)  //change background only for cell(1,2)
//        {
//            QBrush redBackground(Qt::red);
//            return redBackground;
//        }
//        break;
    case Qt::TextAlignmentRole:

        if (col == 5) //change text alignment only for cell(1,1)
        {
            return Qt::AlignCenter + Qt::AlignVCenter;
        }
        break;
    case Qt::CheckStateRole:

        if (col == 5) //add a checkbox to cell(1,0)
        {
            return m_Inputs[row].skip == "01";
        }
    }
    return QVariant();

}

bool WiringModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {

    }
    else if (role == Qt::CheckStateRole)
    {
        if (!index.isValid())
            return false;
        if (index.column() == 5)
        {
            //return Qt::Checked or Qt::Unchecked here
            //qDebug() << value;
            if ((Qt::CheckState)value.toInt() == Qt::Checked)
            {
                //user has checked item
                m_Inputs[index.column()].skip = "01";
                //        emit editCompleted( result );
                return true;
            }
            else
            {
                //user has unchecked item
                m_Inputs[index.column()].skip = "00";
                //        emit editCompleted( result );
                return true;
            }
        }
    }

//    if (role == Qt::EditRole)
//    {
//        //save value from editor to member m_gridData
//        m_gridData[index.row()][index.column()] = value.toString();
//        //for presentation purposes only: build and emit a joined string
//        QString result;
//        for(int row= 0; row < 4; row++)
//        {
//            for(int col= 0; col < 4; col++)
//            {
//                result += m_gridData[row][col] + " ";
//            }
//        }
//        emit editCompleted( result );
//    }
    return true;
}

Qt::ItemFlags WiringModel::flags(const QModelIndex & index) const
{
    if (index.column() == 0)
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (index.column() == 5)
        return Qt::ItemIsSelectable |  /*Qt::ItemIsEditable |*/ Qt::ItemIsUserCheckable | Qt::ItemIsEnabled ;
    return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
}

QVariant WiringModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) {
            switch (section)
            {
            case 0:
                return QString("Input");
            case 1:
                return QString("Name");
            case 2:
                return QString("Audio");
            case 3:
                return QString("Video");
            case 4:
                return QString("HDMI");
            case 5:
                return QString("Skip");
            }
        }
    }
    return QVariant();
}

//void WiringModel::refreshView()
//{
//    //QModelIndex left = createIndex(0, 0);
//    //QModelIndex right = createIndex(m_Inputs.size() -1, 6);

//    beginInsertRows( QModelIndex(), 0, m_Inputs.size() -1 );
//    endInsertRows();
//    //emit resizeToContents();
//    //emit dataChanged(left, right);
//}

void WiringModel::FillTable(QVector<InputContainer> &inputs)
{
    clear();
    beginInsertRows(QModelIndex(), 0, inputs.size() - 1);
    foreach(InputContainer input, inputs)
        m_Inputs.append(input);
    endInsertRows();
}

void WiringModel::clear() {
   this->beginResetModel();
   m_Inputs.clear();
   this->endResetModel();
}
