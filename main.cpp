#include "editorjson.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EditorJson editor;
    editor.show();

#if 0

    QFile file_in("test.json");
    if(file_in.open(QFile::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file_in.readAll());

        QJsonObject obj = doc.object();
        editor.setJson(obj);
    }



    QJsonObject json = editor.convertToJson();
    QFile file_out("test_out.json");
    if(file_out.open(QFile::WriteOnly|QFile::Truncate))
    {
        QJsonDocument doc(json);
        file_out.write(doc.toJson());
        file_out.flush();
        file_out.close();


        qDebug()<<"Writing Json : \n\n"<<json;
    }
#endif
    return a.exec();
}
