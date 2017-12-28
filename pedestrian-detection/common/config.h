#ifndef CONFIG_H
#define CONFIG_H
#include "tool1.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
class FileDataBase{
    QByteArray data;
    QString config_filename;
public:
    FileDataBase(QString name):config_filename(name)
    {
        load_config_from_file(config_filename);
    }
    ~FileDataBase()
    {

    }
    QByteArray get()
    {
        return data;
    }
    void set(const QByteArray &d)
    {
        data=d;
        save_config_to_file();
    }


private:
    int load_config_from_file()
    {

        QFile *f=new QFile(config_filename);
        bool ret = f->open(QIODevice::ReadOnly);
        if(!ret){
            delete f;
            return 0;
        }
        data=f->readAll();
        f->close();
        return 1;
    }
    int load_config_from_file(QString file_name)
    {

        QFile *f=new QFile(file_name);
        bool ret = f->open(QIODevice::ReadOnly);
        if(!ret){
            delete f;
            return 0;
        }
        data=f->readAll();
        f->close();
        return 1;
    }
    void save_config_to_file()
    {
        QFile *f=new QFile(config_filename);
        bool ret = f->open(QIODevice::ReadWrite|QIODevice::Truncate);
        if(!ret){
            prt(info,"fail to open %s",config_filename.toStdString().data());
            delete f;
        }
        f->write(data);
        f->close();
    }
    void save_config_to_file(QString file_name)
    {
        QFile *f=new QFile(file_name);
        bool ret = f->open(QIODevice::ReadWrite|QIODevice::Truncate);
        if(!ret){
            prt(info,"fail to open %s",file_name.toStdString().data());
            delete f;
        }
        f->write(data);
        f->close();
    }
};
class CameraConfiguration{
public:
    /*
        config save in cfg(config_t),which is load from p_database(FileDatabase).
    */
    FileDataBase *p_database;
    typedef struct camera_config{
        QString ip;
        int port;
    }camera_config_t;
    typedef struct config{
        int camera_amount;
        QList<camera_config_t> camera;
    }config_t;
    config_t cfg;

    CameraConfiguration(QString name)
    {
        p_database=new FileDataBase(name);
        reload_cfg();
        //        QByteArray b=p_database->get();
        //        cfg=decode_from_json(b);
    }
    ~CameraConfiguration()
    {
        delete p_database;
    }
    void set_config(QByteArray &ba)
    {
        p_database->set(ba);
        reload_cfg();
    }

    void set_config(const char *buf)
    {
        QByteArray ba;
        ba.clear();
        ba.append(buf);
        p_database->set(ba);
        reload_cfg();
    }

    QByteArray get_config()
    {
        QByteArray ba=p_database->get();
        return ba;
    }

    void del_camera(int index)
    {
        cfg.camera_amount--;
        cfg.camera.removeAt(index-1);
        save();
    }
    void append_camera(QString url)
    {
        cfg.camera_amount++;
        camera_config_t tmp;
        tmp.ip=url;
        tmp.port=1234;
        cfg.camera.append(tmp);
        save();
    }
    //    camera_config_t get_camera_config(int index)
    //    {
    //        if(index>0&&index<=cfg.camera_amount)
    //          return cfg.camera[index-1];

    //    }
    //    camera_config_t get_camera_config()
    //    {
    //        if(0<cfg.camera_amount)
    //          return cfg.camera[cfg.camera_amount-1];
    //        else
    //            return NULL;
    //    }
private:
    void reload_cfg()
    {
        QByteArray b=p_database->get();
        cfg=decode_from_json(b);
    }

    void save()
    {
        p_database->set(encode_to_json(cfg));
    }
    /*
        parse structure from data
    */
    config_t decode_from_json(QByteArray &json_src)
    {
        QJsonDocument json_doc=QJsonDocument::fromJson(json_src);
        QJsonObject root_obj=json_doc.object();
        config_t data;
        data.camera.clear();
        data.camera_amount=get_int(root_obj,"camera_total_number");
        QJsonArray cams=get_child_array(root_obj,"camera");

        foreach (QJsonValue v, cams) {
            QJsonObject obj=v.toObject();
            camera_config_t t;
            t.ip=get_string(obj,"ip");
            t.port=get_int(obj,"port");
            data.camera.append(t);
        }
        return data;
    }
    /*
        pack data from structure
    */
    QByteArray encode_to_json(config_t data)
    {
        QJsonDocument json_doc_new;
        QJsonObject root_obj;

        root_obj["camera_total_number"]=data.camera_amount;
        QJsonArray cams;

        for(int i=0;i<data.camera_amount;i++)
        {
            QJsonObject o;
            o["ip"]=data.camera[i].ip;
            o["port"]=data.camera[i].port;
            cams.append(o);
        }
        root_obj["camera"]=cams;
        json_doc_new.setObject(root_obj);
        return json_doc_new.toJson();

    }

    inline int get_int(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toInt();
    }
    inline QString get_string(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toString();
    }
    inline bool get_bool(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toBool();
    }
    inline QJsonObject get_child_obj(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toObject();
    }
    inline QJsonArray get_child_array(QJsonObject obj,const char *member_name)
    {
        return obj[member_name].toArray();
    }
};
#endif
