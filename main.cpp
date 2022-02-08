#include "editorjson.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorJson w;
    w.show();


    QFile file("test.json");
    if(file.open(QFile::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

        QJsonObject obj = doc.object();
        w.setJson(obj);
    }
    return a.exec();
}
