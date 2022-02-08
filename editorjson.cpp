#include "editorjson.h"
#include "ui_editorjson.h"

EditorJson::EditorJson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditorJson)
{
    ui->setupUi(this);

    m_main_layout = new QVBoxLayout(this);

    m_root = new QTreeWidget(this);
    QStringList headers;
    headers.append("Key");
    headers.append("Val");
    m_root->setHeaderLabels(headers);

    m_main_layout->addWidget(m_root);

#ifdef KEY_NOT_EDITABLE
    m_root->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(m_root,&QTreeWidget::itemDoubleClicked,[this](QTreeWidgetItem * item, int column){
        if(column == val_column)
        {
            m_root->editItem(item,column);
        }
    });
#endif
    m_save = new QPushButton("Save",this);
    m_main_layout->addWidget(m_save);
}


//------------------------------------------------------
/**
 * @brief EditorJson::~EditorJson
 */
EditorJson::~EditorJson()
{
    delete ui;
}

//------------------------------------------------------
/**
 * @brief EditorJson::setJson
 * @param json
 */
void EditorJson::setJson(const QJsonObject &json)
{
    for(const QString& key :  json.keys()) {

        QJsonValue value = json.value(key);
        QTreeWidgetItem* root_item = new QTreeWidgetItem(m_root);
        root_item->setText(key_column,key);

        if(value.isObject())
        {
            convertJson(value.toObject(),root_item);
        }
        else if(value.isArray())
        {
            convertArray(value.toArray(),root_item,key);
        }
        else
        {
            root_item->setFlags(root_item->flags() | Qt::ItemIsEditable);
            root_item->setData(val_column,0,value.toVariant());
        }
    }
    resizeColumns();
}

//------------------------------------------------------
/**
 * @brief EditorJson::convertJson
 * @param json
 * @param root
 */
void EditorJson::convertJson(const QJsonObject &json, QTreeWidgetItem *root)
{
    for(const QString& key :  json.keys()) {
        QJsonValue value = json.value(key);

        QTreeWidgetItem* root_item = new QTreeWidgetItem(root);
        root_item->setText(key_column,key);
        if(value.isObject())
        {
            convertJson(value.toObject(),root_item);
        }
        else if(value.isArray())
        {
            convertArray(value.toArray(),root_item,key);
        }
        else
        {
            root_item->setFlags(root_item->flags() | Qt::ItemIsEditable);
            root_item->setData(val_column,0,value.toVariant());
        }
    }

}
//------------------------------------------------------
/**
 * @brief EditorJson::convertArray
 * @param array
 * @param root
 * @param array_key
 */
void EditorJson::convertArray(const QJsonArray &array, QTreeWidgetItem *root, const QString array_key)
{
    for(const QJsonValue& value :  array) {


        QTreeWidgetItem* root_item = new QTreeWidgetItem(root);
        root_item->setText(key_column,array_key);
        if(value.isObject())
        {
            convertJson(value.toObject(),root_item);
        }
        else if(value.isArray())
        {
            convertArray(value.toArray(),root_item,array_key);
        }
        else
        {
            root_item->setFlags(root_item->flags() | Qt::ItemIsEditable);
            root_item->setData(val_column,0,value.toVariant());
        }
    }
}
//------------------------------------------------------
/**
 * @brief EditorJson::resizeColumns
 */
void EditorJson::resizeColumns()
{
    for(int i = 0; i < m_root->columnCount(); i++)
        m_root->resizeColumnToContents(i);
}

