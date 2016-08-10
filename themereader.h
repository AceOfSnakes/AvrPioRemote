#ifndef THEMEREADER_H
#define THEMEREADER_H

#include <QString>
#include <QVector>

class ThemeReader
{
public:
    ThemeReader();
    bool readTheme(QString theme);
    struct ThemeElement {
        QString name;
        int pos_x;
        int pos_y;
        int width;
        int height;
        int padding_left;
        int padding_right;
        int padding_top;
        int padding_bottom;
        QString background_image;
        QString style;
    };
    ThemeElement getElement(QString name);
private:
    QVector<ThemeElement> themeElements;
};

#endif // THEMEREADER_H
