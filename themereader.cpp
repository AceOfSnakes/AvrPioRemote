#include "themereader.h"
#include <QFile>
#include <QDomDocument>
#include <QDomNamedNodeMap>
#include <QCoreApplication>
#include <QDebug>

ThemeReader::ThemeReader()
{

}

bool ThemeReader::readTheme(QString theme)
{
    theme = QCoreApplication::applicationDirPath() + "/" + theme;
    QDomDocument doc;
    QFile file(theme);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << theme << "not found";
        return false;
    }
    if (!doc.setContent(&file)) {
        return false;
    }
    file.close();

    QDomNodeList roots = doc.elementsByTagName("theme");
    bool rootFound = false;
    for (int i = 0; i < roots.size(); i++) {
        QDomNode root = roots.item(i);
        QDomNamedNodeMap attributes = root.attributes();
        // name
        QDomNode tmp = attributes.namedItem("name");
        if (tmp.isNull())
            continue;
        theme_name = tmp.nodeValue().trimmed();
        if (theme_name == "")
            continue;
        rootFound = true;
        // padding left
        tmp = attributes.namedItem("padding-left");
        padding_left = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding right
        tmp = attributes.namedItem("padding-right");
        padding_right = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding top
        tmp = attributes.namedItem("padding-top");
        padding_top = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding bottom
        tmp = attributes.namedItem("padding-bottom");
        padding_bottom = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // background image
        tmp = attributes.namedItem("background-image");
        background_image = (!tmp.isNull()) ? tmp.nodeValue().replace("$DIR$", QCoreApplication::applicationDirPath()) : "";

        break;
    }
    if (!rootFound) {
        return false;
    }

    QDomNodeList elements = doc.elementsByTagName("element");
    for (int i = 0; i < elements.size(); i++) {
        ThemeElement te;
        QDomNode element = elements.item(i);
        QDomNamedNodeMap attributes = element.attributes();
        // name
        QDomNode tmp = attributes.namedItem("name");
        if (tmp.isNull())
            continue;
        te.name = tmp.nodeValue().trimmed();
        if (te.name == "")
            continue;
        // parent
        tmp = attributes.namedItem("parent");
        te.parent = (!tmp.isNull()) ? tmp.nodeValue() : "";
        if (te.parent != "") {
            // remember name and parent
            QString name = te.name;
            QString parent = te.parent;
            // find parent
            te = getElement(te.parent);
            if (te.name == "")
                continue; // parent not found
            // write name and parent back
            te.name = name;
            te.parent = parent;
        }
        // qt object name/id
        tmp = attributes.namedItem("id");
        te.id = (!tmp.isNull()) ? tmp.nodeValue() : "";
        // x pos
        tmp = attributes.namedItem("pos-x");
        if ((!tmp.isNull())) {
            te.posX = tmp.nodeValue().toInt();
            te.hasPosX = true;
        } else {
            te.posX = 0;
            te.hasPosX = false;
        }
        // y pos
        tmp = attributes.namedItem("pos-y");
        if ((!tmp.isNull())) {
            te.posY = tmp.nodeValue().toInt();
            te.hasPosY = true;
        } else {
            te.posY = 0;
            te.hasPosY = false;
        }
        // width
        tmp = attributes.namedItem("width");
        if ((!tmp.isNull())) {
            te.width = tmp.nodeValue().toInt();
            te.hasWidth = true;
        } else {
            te.width = 0;
            te.hasWidth = false;
        }
        // height
        tmp = attributes.namedItem("height");
        if ((!tmp.isNull())) {
            te.height = tmp.nodeValue().toInt();
            te.hasHeight = true;
        } else {
            te.height = 0;
            te.hasHeight = false;
        }

        QDomElement style = element.firstChildElement("style");
        if (!style.isNull()) {
            te.style = style.text().trimmed().replace("$DIR$", QCoreApplication::applicationDirPath());
        }
        themeElements.append(te);
    }
    return true;
}

ThemeReader::ThemeElement ThemeReader::getElement(QString name)
{
    ThemeElement el;
    foreach (ThemeElement el, themeElements) {
        if (el.name == name)
            return el;
    }
    return el;
}

void ThemeReader::applyThemeToWidgets(QWidget* parent)
{
    foreach(const ThemeElement& el, themeElements) {
        if (el.id != "") {
            QWidget* obj = parent->findChild<QWidget*>(el.id);
            if (obj != NULL) {
                applyThemeToWidget(obj, el);
            }
        }
    }
}

void ThemeReader::applyThemeToWidget(QWidget* widget, const ThemeElement& te)
{
    if (te.hasWidth)
        widget->setFixedWidth(te.width);
    if (te.hasHeight)
        widget->setFixedHeight(te.height);
    if (te.hasPosX || te.hasPosY) {
        QPoint pos = widget->pos();
        if (te.hasPosX)
            pos.setX(te.posX);
        if (te.hasPosY)
            pos.setY(te.posY);
        widget->move(pos);
    }
    if (te.style != "") {
        widget->setStyleSheet(te.style);
    }

}
