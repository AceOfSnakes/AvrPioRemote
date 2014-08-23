#ifndef WIRINGMODEL_H
#define WIRINGMODEL_H

#include <QAbstractTableModel>
#include <QModelIndex>
#include <QVector>
#include <QMap>

struct InputContainer
{
    InputContainer()
    {
    }
    InputContainer(const QString& id, const QString& name)
    {
        this->id = id;
        this->name = name;
    }

    QString id;
    QString name;
    QString alias;
    QString audio;
    QString hdmi;
    QString video;
    QString skip;
};


class WiringModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit WiringModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex & index) const ;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    //void refreshView();
    void FillTable(QVector<InputContainer>& inputs);
    void clear();

    QVector<InputContainer>     m_Inputs;
    QMap<QString, QString>      m_AudioList;
    QMap<QString, QString>      m_VideoList;
    QMap<QString, QString>      m_HDMIList;

private:
signals:
    void editCompleted(const QString &);
};

#endif // WIRINGMODEL_H
