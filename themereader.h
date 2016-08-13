#ifndef THEMEREADER_H
#define THEMEREADER_H

#include <QWidget>
#include <QString>
#include <QVector>

class ThemeReader
{
public:
    ThemeReader();
    bool readTheme(QString theme);
    struct ThemeElement {
        QString name;
        QString id;
        QString parent;
        bool hasPosX;
        int posX;
        bool hasPosY;
        int posY;
        bool hasWidth;
        int width;
        bool hasHeight;
        int height;
        QString style;
    };
    QString theme_name;
    QString background_image;
    int padding_left;
    int padding_right;
    int padding_top;
    int padding_bottom;
    ThemeElement getElement(QString name);
    void applyThemeToWidgets(QWidget* parent);
    void applyThemeToWidget(QWidget* widget, const ThemeElement& te);
private:
    QVector<ThemeElement> themeElements;
};

#endif // THEMEREADER_H
