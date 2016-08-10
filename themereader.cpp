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
        // x pos
        tmp = attributes.namedItem("pos-x");
        te.pos_x = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // y pos
        tmp = attributes.namedItem("pos-y");
        te.pos_y = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // width
        tmp = attributes.namedItem("width");
        te.width = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // height
        tmp = attributes.namedItem("height");
        te.height = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding left
        tmp = attributes.namedItem("padding-left");
        te.padding_left = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding right
        tmp = attributes.namedItem("padding-right");
        te.padding_right = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding top
        tmp = attributes.namedItem("padding-top");
        te.padding_top = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // padding bottom
        tmp = attributes.namedItem("padding-bottom");
        te.padding_bottom = (!tmp.isNull()) ? tmp.nodeValue().toInt() : 0;
        // name
        tmp = attributes.namedItem("name");
        te.name = (!tmp.isNull()) ? tmp.nodeValue() : "";
        // background image
        tmp = attributes.namedItem("background-image");
        te.background_image = (!tmp.isNull()) ? tmp.nodeValue().replace("$DIR$", QCoreApplication::applicationDirPath()) : "";

        QDomElement style = element.firstChildElement("style");
        if (!style.isNull()) {
            te.style = style.text().trimmed().replace("$DIR$", QCoreApplication::applicationDirPath());
        }
        themeElements.append(te);
//        QDomNodeList children = element.childNodes();
//        for (int j = 0; j < children.size(); j++) {
//            QDomNode node = children.at(j);
//            qDebug() << node.nodeName();
//        }
        //qDebug() << element.
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
