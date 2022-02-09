#include "editorjson.h"
#include "ui_editorjson.h"

EditorJson::EditorJson(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditorJson)
{
    ui->setupUi(this);

    m_main_layout = new QVBoxLayout(this);

    QString home_path = QDir::homePath();

#ifdef USE_RFTE_HOME_DIR
    home_path = QDir::homePath()+QString("/RFTE/config");
#endif


    m_root = new QTreeWidget(this);
    QStringList headers;
    headers.append("Key");
    headers.append("Val");
    m_root->setHeaderLabels(headers);
    m_root->setAcceptDrops(true);

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

    QIcon save_icon(":/icons/save.png");


    m_save = new QPushButton("Save",this);
    m_save->setIcon(save_icon);
    m_main_layout->addWidget(m_save);

    connect(m_save,&QPushButton::clicked,[this,home_path](){


        QString dest_file = QFileDialog::getSaveFileName(this,
                                                         tr("Save"),
                                                         home_path,
                                                         tr("json (*.json)"));
        saveToFile(dest_file);

    });





#ifdef CLEAR_BTN_TEST

    m_clear = new QPushButton("Clear",this);
    m_main_layout->addWidget(m_clear);

    connect(m_clear,&QPushButton::clicked,[this](){
        for (int i = 0; i < m_root->topLevelItemCount(); ++i) {
            QTreeWidgetItem *item = m_root->topLevelItem( i );

            delete item;
        }
        m_root->clear();
    });

#endif




    QShortcut* open_file_short_cut = new QShortcut(QKeySequence(SHORTCUT_OPEN_FILE),this);


    connect(open_file_short_cut,&QShortcut::activated,[this,home_path](){

        QString fileName = QFileDialog::getOpenFileName(this,tr("Open the json file"), home_path, tr("Json Files (*.json)"));
        if(fileName.isEmpty())
            return;
        else
        {
            qDebug()<<"Selected file : "<<fileName;
            setJson(fileName);

        }

    });
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
 * @brief EditorJson::convertToJson
 * @param root
 */
QJsonObject EditorJson::convertToJsonFromJsonObject(QTreeWidgetItem *root)
{
    QJsonObject res;

#ifdef PRINT_DEBUG
    qDebug()<<"Root - Key: "<<root->data(key_column,0)<<" Type : "<<map_data_type_name[static_cast<teDataType>(root->data(key_column,Qt::UserRole).toInt())];
#endif

    for( int i = 0; i < root->childCount(); ++i )
    {
        QTreeWidgetItem *item = root->child(i );
        teDataType data_type = static_cast<teDataType>(item->data(key_column,Qt::UserRole).toInt());
        QString item_key =   item->data(key_column,0).toString();

        //parsing
        if(data_type == teDataType::OBJECT)
        {
            QJsonObject obj =  convertToJsonFromJsonObject(item);
            res.insert(item_key,obj);
        }
        else if(data_type == teDataType::ARRAY)
        {
            QJsonArray array = convertToJsonFromJsonArray(item);
            res.insert(item_key,array);
        }
        else
        {
            //res.insert(item_key,QJsonValue::fromVariant(item->data(val_column,0)));
            saveValue(res,item_key,item->data(val_column,0));
#ifdef PRINT_DEBUG
            qDebug()<<"Root - Key: "<<item->data(key_column,0)<<" Type : "<<map_data_type_name[data_type]<<" Value "<<item->data(val_column,0);
#endif
        }


    }//for loop



    return res;
}
//------------------------------------------------------
/**
 * @brief EditorJson::convertToJsonFromJsonArray
 * @param root
 * @param parent
 */
QJsonArray EditorJson::convertToJsonFromJsonArray(QTreeWidgetItem *root)
{
    QJsonArray res;
#ifdef PRINT_DEBUG
    qDebug()<<"Root - Key: "<<root->data(key_column,0)<<" Type : "<<map_data_type_name[static_cast<teDataType>(root->data(key_column,Qt::UserRole).toInt())];
#endif
    for( int i = 0; i < root->childCount(); ++i )
    {
        QTreeWidgetItem *item = root->child(i );
        teDataType data_type = static_cast<teDataType>(item->data(key_column,Qt::UserRole).toInt());
        QString item_key =   item->data(key_column,0).toString();

        //parsing
        if(data_type == teDataType::OBJECT)
        {
            QJsonObject obj =  convertToJsonFromJsonObject(item);
            res.push_back(obj);
        }
        else if(data_type == teDataType::ARRAY)
        {
            QJsonArray array =  convertToJsonFromJsonArray(item);
            res.push_back(array);
        }
        else
        {
            saveValue(res,item->data(val_column,0));
            //res.push_back(QJsonValue::fromVariant(item->data(val_column,0)));
#ifdef PRINT_DEBUG
            qDebug()<<"Root - Key: "<<item->data(key_column,0)<<" Type : "<<map_data_type_name[data_type]<<" Value "<<item->data(val_column,0);
#endif
        }


    }//for loop



    return res;
}


//------------------------------------------------------
/**
 * @brief EditorJson::convertToJson
 */
QJsonObject EditorJson::convertToJson()
{
    QJsonObject res;
    for( int i = 0; i < m_root->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem *item = m_root->topLevelItem( i );
        teDataType data_type = static_cast<teDataType>(item->data(key_column,Qt::UserRole).toInt());
        QString item_key =   item->data(key_column,0).toString();

        //parsing
        if(data_type == teDataType::OBJECT)
        {
            QJsonObject obj =  convertToJsonFromJsonObject(item);
            res.insert(item_key,obj);
        }
        else if(data_type == teDataType::ARRAY)
        {
            QJsonArray array = convertToJsonFromJsonArray(item);
            res.insert(item_key,array);

        }
        else
        {
//            res.insert(item_key,QJsonValue::fromVariant(item->data(val_column,0)));
            saveValue(res,item_key,item->data(val_column,0));
#ifdef PRINT_DEBUG
            qDebug()<<"Root - Key: "<<item->data(key_column,0)<<" Type : "<<map_data_type_name[data_type]<<" Value "<<item->data(val_column,0);
#endif
        }

    }

    return res;
}
//------------------------------------------------------
/**
 * @brief EditorJson::saveToFile
 * @param path_file
 * @return
 */
QJsonObject EditorJson::saveToFile(const QString &path_file)
{
    QString path{m_path_file};
    if(!path_file.isEmpty())
        path = path_file;


    QJsonObject json;
    if(!path.isEmpty())
    {
       json = convertToJson();
        QFile file_out(path_file);
        if(file_out.open(QFile::WriteOnly|QFile::Truncate))
        {
            QJsonDocument doc(json);
            file_out.write(doc.toJson());
            file_out.flush();
            file_out.close();
        }
    }
    else
    {
        QMessageBox::warning(this,"Error","No destination file detected!");
        qDebug()<<"Error : no output path detected! ";
    }

    return json;
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
        root_item->setData(key_column,0,key);

        if(value.isObject())//pbject
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::OBJECT)));
            convertFromJsonObject(value.toObject(),root_item);
        }
        else if(value.isArray()) //array
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::ARRAY)));
            convertFromJsonArray(value.toArray(),root_item,key);
        }
        else//value
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::VALUE)));
            root_item->setFlags(root_item->flags() | Qt::ItemIsEditable);
            root_item->setData(val_column,0,value.toVariant());
        }
    }
    resizeColumns();
}
//------------------------------------------------------
/**
 * @brief EditorJson::setJson
 * @param path_file
 */
void EditorJson::setJson(const QString &path_file)
{
    m_root->clear();
    if(!path_file.isEmpty())
    {
        m_path_file = path_file;
        QFile file_in(m_path_file);
        if(file_in.open(QFile::ReadOnly))
        {
            QJsonDocument doc = QJsonDocument::fromJson(file_in.readAll());
            setJson(doc.object());
        }
    }
}

//------------------------------------------------------
/**
 * @brief EditorJson::convertJson
 * @param json
 * @param root
 */
void EditorJson::convertFromJsonObject(const QJsonObject &json, QTreeWidgetItem *root)
{
    for(const QString& key :  json.keys()) {
        QJsonValue value = json.value(key);

        QTreeWidgetItem* root_item = new QTreeWidgetItem(root);
        root_item->setData(key_column,0,key);
        if(value.isObject())
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::OBJECT)));
            convertFromJsonObject(value.toObject(),root_item);
        }
        else if(value.isArray())
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::ARRAY)));
            convertFromJsonArray(value.toArray(),root_item,key);
        }
        else
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::VALUE)));
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
void EditorJson::convertFromJsonArray(const QJsonArray &array, QTreeWidgetItem *root, const QString array_key)
{
    for(const QJsonValue& value :  array) {


        QTreeWidgetItem* root_item = new QTreeWidgetItem(root);
        root_item->setData(key_column,0,array_key);
        if(value.isObject())
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::OBJECT)));
            convertFromJsonObject(value.toObject(),root_item);
        }
        else if(value.isArray())
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::ARRAY)));
            convertFromJsonArray(value.toArray(),root_item,array_key);
        }
        else
        {
            root_item->setData(key_column,Qt::UserRole,QVariant(static_cast<int>(teDataType::VALUE)));
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
//------------------------------------------------------
/**
 * @brief EditorJson::saveValue
 * @param obj
 * @param val
 */
void EditorJson::saveValue(QJsonObject &obj, const QString &key, QVariant val)
{
    if(isInteger(val))
    {
        obj.insert(key,val.toInt());
    }
    else if(isBool(val))
    {
        obj.insert(key,val.toBool());
    }
    else if(isDouble(val))
    {
        obj.insert(key,val.toDouble());
    }
    else if(isLongLong(val))
    {
        obj.insert(key,val.toLongLong());
    }
    else
    {
        obj.insert(key,val.toString());
    }
}
//------------------------------------------------------
/**
 * @brief EditorJson::saveValue
 * @param array
 * @param key
 * @param val
 */
void EditorJson::saveValue(QJsonArray &array, QVariant val)
{
    if(isInteger(val))
    {
        array.push_back(val.toInt());
    }
    else if(isBool(val))
    {

        array.push_back(val.toBool());
    }
    else if(isDouble(val))
    {
        array.push_back(val.toDouble());
    }
    else if(isLongLong(val))
    {
        array.push_back(val.toLongLong());
    }
    else
    {
        array.push_back(val.toString());
    }
}
//------------------------------------------------------
/**
 * @brief EditorJson::isInteger
 * @param variant
 * @return
 */
bool EditorJson::isInteger(const QVariant &variant)
{
    return ((variant.userType() == QMetaType::Int) || (variant.userType() == QMetaType::UInt) ) ;
}
//------------------------------------------------------
/**
 * @brief EditorJson::isLongLong
 * @param variant
 * @return
 */
bool EditorJson::isLongLong(const QVariant &variant)
{
    return ((variant.userType() == QMetaType::LongLong) || (variant.userType() == QMetaType::ULongLong) ) ;
}
//------------------------------------------------------
/**
 * @brief EditorJson::isDouble
 * @param variant
 * @return
 */
bool EditorJson::isDouble(const QVariant &variant)
{
    return variant.userType() == QMetaType::Double;

}
//------------------------------------------------------
/**
 * @brief EditorJson::isBool
 * @param variant
 * @return
 */
bool EditorJson::isBool(const QVariant &variant)
{
    return variant.userType() == QMetaType::Bool;

}
//------------------------------------------------------
/**
 * @brief EditorJson::isString
 * @param variant
 * @return
 */
bool EditorJson::isString(const QVariant &variant)
{
    return variant.userType() == QMetaType::QString;

}
