#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    connect(ui->settings_list,
            &QListWidget::currentRowChanged,
            ui->stack_widget,
            &QStackedWidget::setCurrentIndex);
    readCtrlSettings();
    readAudioSettings();
    readAdvancedSettings();

    connect(ui->enable_rand_mode,&QCheckBox::stateChanged,this,&Settings::enableRandModeSetted);
    readPlayerData();
    readBoxData();
    readLevelData();
}

Settings::~Settings()
{
    writeCtrlSettings();
    writeAudioSettings();
    writeAdvancedSettings();

    for(auto obj:roles){
        delete obj;
    }
    for(auto box:boxes){
        delete box;
    }
    for(auto& item:levels)
    {
        for(auto& obj:item)
        {
            delete obj;
        }
    }
    delete ui;
}

const QVector<QJsonObject*> Settings::getLevelsInMode(gameMain::gameMode mode) const
{
    if(mode == gameMain::singleMode)
    {
        return levels["single"];
    }
    else
    {
        return levels["multi"];
    }
}

void Settings::setEnableRandMode(bool flag)
{
    ui->enable_rand_mode->setChecked(flag);
    emit enableRandModeSetted(flag);
}

void Settings::on_ok_button_clicked()
{
    this->close();
}

void Settings::readPlayerData()
{
    QFile reader;
    //读取玩家索引文件
    reader.setFileName("settings/roles/playerlist.json");
    reader.open(QIODevice::ReadOnly);
    auto jArray = QJsonDocument::fromJson(reader.readAll()).array();
    QStringList players;

    //读取所有玩家id，便于查找
    int jSize = jArray.size();
    for(int i = 0;i <= jSize - 1;++i)
    {
        players.append(jArray.at(i).toString());
    }
    reader.close();

    QJsonObject* handle;
    for(auto id:players)//读取playerlist.json文件中每个玩家对应的文件
    {
        reader.setFileName("settings/roles/" + id + ".json");
        reader.open(QIODevice::ReadOnly);
        handle = new QJsonObject(QJsonDocument::fromJson(reader.readAll()).object());
        roles.push_back(handle);
        reader.close();
    }
}
void Settings::readBoxData()
{
    QFile reader;
    //读取箱子索引文件
    reader.setFileName("settings/boxes/boxlist.json");
    reader.open(QIODevice::ReadOnly);
    auto jArray = QJsonDocument::fromJson(reader.readAll()).array();
    reader.close();

    QJsonObject* handle;
    //读取每个箱子data
    for(auto id:jArray){
        handle = new QJsonObject;
        handle->insert("id",id);
        reader.setFileName("settings/boxes/" + id.toString() + ".json");
        reader.open(QIODevice::ReadOnly);
        //merge object
        QJsonObject box = QJsonDocument::fromJson(reader.readAll()).object();
        reader.close();
        for(const auto& key:box.keys()){
            handle->insert(key,box[key]);
        }
        boxes.push_back(handle);
    }
}
void Settings::readLevelData()
{
    QFile reader;
    QJsonObject* handle;
    QDir dir;
    for(const QString& item:{"single","multi"})
    {
        dir.setPath("levels/" + item);
        levels.insert(item,QVector<QJsonObject*>());
        for(int i = 1;i + 2 <= dir.count();++i)
        {
            handle = new QJsonObject;
            //基本设置文件basic.json
            reader.setFileName(dir.path() + "/" + QString::number(i) + "/basic.json");
            reader.open(QIODevice::ReadOnly);
            handle->insert("basic",QJsonDocument::fromJson(reader.readAll()).object());
            reader.close();

            //地图文件map.json
            reader.setFileName(dir.path() + "/" + QString::number(i) + "/map.json");
            reader.open(QIODevice::ReadOnly);
            handle->insert("map",QJsonDocument::fromJson(reader.readAll()).array());
            reader.close();

            //玩家信息文件player_infos.json
            reader.setFileName(dir.path() + "/" + QString::number(i) + "/player_infos.json");
            reader.open(QIODevice::ReadOnly);
            handle->insert("playerInfos",QJsonDocument::fromJson(reader.readAll()).array());
            reader.close();

            levels[item].push_back(handle);
        }
    }
}
void Settings::readCtrlSettings()
{
    QFile reader;
    //控制设置
    reader.setFileName("settings/control_settings.json");
    reader.open(QIODevice::ReadOnly);
    QJsonObject ctrlSettings(QJsonDocument::fromJson(reader.readAll()).object());
    QJsonArray keyPressed = ctrlSettings.value("keyPressed").toArray();
    //第一个玩家
    ui->p1_key_up->setKey(keyPressed.at(0).toObject().value("up").toInt());
    ui->p1_key_down->setKey(keyPressed.at(0).toObject().value("down").toInt());
    ui->p1_key_left->setKey(keyPressed.at(0).toObject().value("left").toInt());
    ui->p1_key_right->setKey(keyPressed.at(0).toObject().value("right").toInt());
    //第二个玩家
    ui->p2_key_up->setKey(keyPressed.at(1).toObject().value("up").toInt());
    ui->p2_key_down->setKey(keyPressed.at(1).toObject().value("down").toInt());
    ui->p2_key_left->setKey(keyPressed.at(1).toObject().value("left").toInt());
    ui->p2_key_right->setKey(keyPressed.at(1).toObject().value("right").toInt());

    reader.close();
}
void Settings::readAudioSettings()
{
    QFile reader;
    //声音设置
    reader.setFileName("settings/audio_settings.json");
    reader.open(QIODevice::ReadOnly);
    QJsonObject audioSettings(QJsonDocument::fromJson(reader.readAll()).object());
    //实体声音设置
    ui->entity_audio_slider->setValue(audioSettings.value("entityAudio").toInt());
    //背景音乐设置
    ui->bgm_audio_slider->setValue(audioSettings.value("bgmAudio").toInt());

    reader.close();
}

void Settings::readAdvancedSettings()
{
    QFile reader;
    reader.setFileName("settings/advanced_settings.json");
    reader.open(QIODevice::ReadOnly);
    QJsonObject advancedSettings(QJsonDocument::fromJson(reader.readAll()).object());
    reader.close();
    setEnableRandMode(advancedSettings["enableRandMode"].toBool());
}
void Settings::writeCtrlSettings()
{
    QFile writer;
    //控制设置
    writer.setFileName("settings/control_settings.json");
    writer.open(QIODevice::WriteOnly);

    //第一个玩家
    QJsonObject p1;
    p1.insert("up",ui->p1_key_up->key());
    p1.insert("down",ui->p1_key_down->key());
    p1.insert("left",ui->p1_key_left->key());
    p1.insert("right",ui->p1_key_right->key());
    //第二个玩家
    QJsonObject p2;
    p2.insert("up",ui->p2_key_up->key());
    p2.insert("down",ui->p2_key_down->key());
    p2.insert("left",ui->p2_key_left->key());
    p2.insert("right",ui->p2_key_right->key());

    QJsonArray keyPressed;
    keyPressed.append(p1);
    keyPressed.append(p2);
    QJsonObject ctrlSettings;
    ctrlSettings.insert("keyPressed",keyPressed);

    writer.write(QJsonDocument(ctrlSettings).toJson());
    writer.close();
}
void Settings::writeAudioSettings()
{
    QFile writer;
    writer.setFileName("settings/audio_settings.json");
    writer.open(QIODevice::WriteOnly);

    QJsonObject audioSettings;
    audioSettings.insert("entityAudio",QJsonValue(ui->entity_audio_slider->value()));
    audioSettings.insert("bgmAudio",QJsonValue(ui->bgm_audio_slider->value()));

    writer.write(QJsonDocument(audioSettings).toJson());
    writer.close();
}

void Settings::writeAdvancedSettings()
{
    QFile writer;
    writer.setFileName("settings/advanced_settings.json");
    writer.open(QIODevice::WriteOnly);

    QJsonObject advancedSettings;
    advancedSettings.insert("enableRandMode",ui->enable_rand_mode->isChecked());

    writer.write(QJsonDocument(advancedSettings).toJson());
    writer.close();
}
