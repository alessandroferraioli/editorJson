#ifndef EDITORJSON_H
#define EDITORJSON_H

#include <QWidget>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>

#include <QLineEdit>
#include <QVBoxLayout>


constexpr int key_column{0};
constexpr int val_column{1};


QT_BEGIN_NAMESPACE
namespace Ui { class EditorJson; }
QT_END_NAMESPACE

class EditorJson : public QWidget
{
    Q_OBJECT

public:
    EditorJson(QWidget *parent = nullptr);

    void setJson(const QJsonObject& json);
    ~EditorJson();

private:
    Ui::EditorJson *ui;
    QVBoxLayout* m_main_layout{nullptr};
    QTreeWidget *m_root{nullptr};
    QPushButton* m_save{nullptr};


    void convertJson(const QJsonObject&json, QTreeWidgetItem* root);
    void convertArray(const QJsonArray& array, QTreeWidgetItem* root,const QString array_key="");

    void resizeColumns();
};
#endif // EDITORJSON_H
