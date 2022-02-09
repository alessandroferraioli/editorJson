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


typedef enum
{
    VALUE = 0,
    OBJECT,
    ARRAY
}teDataType;

static const QMap<teDataType,QString> map_data_type_name{
    {teDataType::VALUE, "Value"},
    {teDataType::OBJECT, "Object"},
    {teDataType::ARRAY, "Array"},
};
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

public slots:
    QJsonObject convertToJson();

private:
    Ui::EditorJson *ui;
    QVBoxLayout* m_main_layout{nullptr};
    QTreeWidget *m_root{nullptr};
    QPushButton* m_save{nullptr};

#ifdef CLEAR_BTN_TEST
    QPushButton* m_clear{nullptr};
#endif


    void convertFromJsonObject(const QJsonObject&json, QTreeWidgetItem* root);
    void convertFromJsonArray(const QJsonArray& array, QTreeWidgetItem* root,const QString array_key="");


    QJsonObject convertToJsonFromJsonObject(QTreeWidgetItem* root);
    QJsonArray convertToJsonFromJsonArray(QTreeWidgetItem* root);

    void resizeColumns();
};
#endif // EDITORJSON_H
