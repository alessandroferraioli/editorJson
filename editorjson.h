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
#include <QShortcut>
#include <QFileDialog>
#include <QMessageBox>

#define SHORTCUT_OPEN_FILE "Ctrl+o"
#define SHORTCUT_SAVE_FILE "Ctrl+s"


typedef enum
{
    VALUE = 0,
    OBJECT,
    ARRAY
}teDataType;


typedef enum
{
    STRING = 0,
    DOUBLE,
    LONGLONG,
    INT,
    BOOL
}teValueType;

#if 0
static const QMap<teDataType,QString> map_data_type_name{
    {teDataType::VALUE, "Value"},
    {teDataType::OBJECT, "Object"},
    {teDataType::ARRAY, "Array"},
};
#endif

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
    void setJson(const QString& path_file);
    ~EditorJson();

public slots:
    QJsonObject convertToJson();
    QJsonObject saveToFile(const QString& path_file ="");

private:
    Ui::EditorJson *ui;
    QVBoxLayout* m_main_layout{nullptr};
    QTreeWidget *m_root{nullptr};
    QPushButton* m_save{nullptr};

    QString m_path_file{""};
#ifdef CLEAR_BTN_TEST
    QPushButton* m_clear{nullptr};
#endif


    void convertFromJsonObject(const QJsonObject&json, QTreeWidgetItem* root);
    void convertFromJsonArray(const QJsonArray& array, QTreeWidgetItem* root,const QString array_key="");


    QJsonObject convertToJsonFromJsonObject(QTreeWidgetItem* root);
    QJsonArray convertToJsonFromJsonArray(QTreeWidgetItem* root);

    void resizeColumns();


    void saveValue(QJsonObject& obj,const QString& key,QVariant val,teValueType type);
    void saveValue(QJsonArray& array,QVariant val,teValueType type);

    bool isInteger(const QVariant& variant);
    bool isLongLong(const QVariant& variant);
    bool isDouble(const QVariant& variant);
    bool isBool(const QVariant& variant);
    bool isString(const QVariant& variant);

    teValueType valueType(const QVariant& val);



};
#endif // EDITORJSON_H
