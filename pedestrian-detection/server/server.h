#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include <QNetworkInterface>
#include "tool1.h"
#include "pd.h"
#include "camera.h"
class ServerInfoReporter : public QObject{
    Q_OBJECT
public:
    ServerInfoReporter(){
        timer=new QTimer();
        connect(timer,SIGNAL(timeout()),this,SLOT(check_client_msg()));//TODO:maybe replace with readReady signal
        udp_skt = new QUdpSocket();
        udp_skt->bind(Protocol::SERVER_REPORTER_PORT,QUdpSocket::ShareAddress);
    }
    ~ServerInfoReporter()
    {
        disconnect(timer);
        delete timer;
        delete udp_skt;
    }
    void start()
    {
        timer->start(1000);
    }

    void stop()
    {
        timer->stop();
    }

public  slots:
    void check_client_msg()
    {
        QByteArray client_msg;
        char *msg;
        if(udp_skt->hasPendingDatagrams())
        {
            client_msg.resize((udp_skt->pendingDatagramSize()));
            udp_skt->readDatagram(client_msg.data(),client_msg.size());
            QString str=udp_skt->peerAddress().toString();//TODO,why cant I get this address?
            prt(info,"get client broadcasted code :%s",msg=client_msg.data());
            if(!strcmp(msg,"pedestrian")){
                prt(info,"reply client %s with our ip info",str.toStdString().data());
                send_info_to_client();
            }
            else{
                prt(error,"client code :%s NOT MATCH pedestrian,we will not answer",msg=client_msg.data());
            }
            //   udp_skt->flush();
        }else{
            //prt(debug,"searching client on port %d",Protocol::SERVER_REPORTER_PORT)
        }
    }

    void send_info_to_client()
    {
        QByteArray datagram;
        datagram.clear();
        QList <QNetworkInterface>list_interface=QNetworkInterface::allInterfaces();
        foreach (QNetworkInterface i, list_interface) {
            if(i.name()!="lo"){
                QList<QNetworkAddressEntry> list_entry=i.addressEntries();
                foreach (QNetworkAddressEntry e, list_entry) {
                    if(e.ip().protocol()==QAbstractSocket::IPv4Protocol)
                    {
                        datagram.append(QString(e.ip().toString())).append(QString(",")).\
                                append(QString(e.netmask().toString())).append(QString(",")).append(QString(e.broadcast().toString()));
                    }

                }
            }
        }
#if 1
        //broadcast
        udp_skt->writeDatagram(datagram.data(), datagram.size(),
                               QHostAddress::Broadcast, Protocol::CLIENT_REPORTER_PORT);
#else
         //send to single ip. problem in windows
#endif
    }
private:
    QTimer *timer;
    QUdpSocket *udp_skt;
};

class ClientSession:public QObject{
    Q_OBJECT
public:
    ClientSession(QTcpSocket *client_skt):skt(client_skt),focus_index(0){

        connect(skt,SIGNAL(readyRead()),this,SLOT(handle_msg()));
        connect(skt,SIGNAL(disconnected()),this,SLOT(deleteLater()));
        connect(skt,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(socket_error()));
     //   udp_skt=new QUdpSocket();
        client_addr=skt->peerAddress();
        timer=new QTimer(this);
        connect(timer,SIGNAL(timeout()),this,SLOT(check_output()));
        timer->start(10);
        //  connect(skt,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(test()));
        //                connect(skt,SIGNAL(aboutToClose()),this,SLOT(test()));
        //                connect(skt,SIGNAL(disconnected()),this,SLOT(test()));
    }
    ~ClientSession()
    {
        delete timer;
     //   //    disconnect(timer,SIGNAL(timeout()),this,SLOT(send_rst_to_client()));
       // delete udp_skt;
    }

public slots:
    void open_output(int index)
    {
        focus_index=index;
    }
    void close_output()
    {
        focus_index=0;
    }
    bool camera_foucused()
    {
        if(focus_index>0)
            return true;
        if(focus_index<0)
            return false;
    }

    void check_output()
    {
        ProcessedDataSender *sender=ProcessedDataSender::get_instance();
        CameraManager &mgr=  CameraManager::GetInstance();
        QByteArray ba;
        if(focus_index&&focus_index<=mgr.cam_num()){
            //prt(info,"checking camera %d",focus_index);
            if(mgr.try_get_data(focus_index-1,ba)){
                sender->send(ba,client_addr);
            }
        }else{
            if(camera_foucused()){
                prt(info,"close camera %d output",focus_index);
                close_output();
            }
        }
    }

    void socket_error()
    {
        emit socket_error(this);
    }

    void handle_msg(){
        int writes_num=0;
        QByteArray client_buf=skt->readAll();
        rcv_buf=client_buf.data();

        int ret_size=process(rcv_buf,send_buf,client_buf.size());
        writes_num=skt->write(send_buf,ret_size);
        prt(info,"server reply %d bytes",writes_num);
    }
    int process(char *src_buf,char*dst_buf,int size)
    {
        CameraManager &mgr=CameraManager::GetInstance();
        prt(debug,"handle client %s cmd",ip().toStdString().data());
        int client_cmd=Protocol::get_operation(src_buf);
        int pkg_len=Protocol::get_length(src_buf);
        int cam_index=Protocol::get_cam_index(src_buf);

        QByteArray bta;
        int ret_size=0;
        switch (client_cmd) {
        case Protocol::ADD_CAMERA:
            prt(info,"client %s request add camera",ip().toStdString().data());
            bta.clear();
            bta.append(src_buf+Protocol::HEAD_LENGTH,pkg_len);
            mgr.add_camera(bta.data());
            memcpy(dst_buf,src_buf,size);
            ret_size= Protocol::HEAD_LENGTH;
            break;
        case  Protocol::GET_CONFIG:
            prt(info,"client %s request fetch configuration",ip().toStdString().data());
            memcpy(dst_buf,src_buf,Protocol::HEAD_LENGTH);
            memcpy(dst_buf+Protocol::HEAD_LENGTH,mgr.p_cfg->get_config().data(),mgr.p_cfg->get_config().size());
            ret_size=mgr.p_cfg->get_config().size()+Protocol::HEAD_LENGTH;
            break;
        case Protocol::DEL_CAMERA:
            prt(info,"client %s request delete camera %d",ip().toStdString().data(),cam_index);
            mgr.del_camera(cam_index);
            memcpy(dst_buf,src_buf,Protocol::HEAD_LENGTH);
            ret_size= Protocol::HEAD_LENGTH;
            break;
        case Protocol::MOD_CAMERA:
            prt(info,"modify cam %d ",cam_index);
            break;
        case Protocol::CAM_OUTPUT_OPEN:
            prt(info,"client %s request camera %d output data",ip().toStdString().data(),cam_index);
            memcpy(dst_buf,src_buf,Protocol::HEAD_LENGTH);
            ret_size= Protocol::HEAD_LENGTH;
          //  focus_index=cam_index;
            open_output(cam_index);
            //mgr.set_output(cam_index);
            break;
        default:
            break;
        }
        return ret_size;
    }
    void displayError(QAbstractSocket::SocketError socketError)
    {
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            prt(info,"err");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            prt(info,"err");
            break;
        default:break;

        }
    }
    QString ip()
    {
        return client_addr.toString();
    }

signals :
    int get_server_config(char *buf);
    void socket_error(ClientSession *c);
private:
    char *rcv_buf;
    char send_buf[Pd::BUFFER_LENGTH];
    QTcpSocket *skt;
  //  QUdpSocket *udp_skt;
    QTimer *timer;
    QHostAddress client_addr;
    int focus_index;
};
/*
    server provide
        tcpserver:handle clients session
        clients:hold all ClientSession
        systeminforeporter:recive udp request,and respond with ip ,netmask ,etc...
        camera manager:holds configuration , runing camera,manage add,delete and other operation

*/
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent=0 ):QObject(parent){
        bool ret=false;
        server=new QTcpServer();
        ret=server->listen(QHostAddress::Any,Protocol::SERVER_PORT);
        if(ret){
            prt(info,"Server listen on port %d success!",Protocol::SERVER_PORT);
        } else {
            prt(fatal,"Server listen on port %d failed!",Protocol::SERVER_PORT);
            exit(1);
        }
        connect(server, &QTcpServer::newConnection, this, &Server::handle_connection);
    }
    ~Server(){
        delete server;
    }
    void print_server_info()
    {
        //     prt(info,"server started ");
    }

signals:
public slots:
    void handle_connection()
    {
        QTcpSocket *skt = server->nextPendingConnection();
        connect(skt, SIGNAL(disconnected()),skt, SLOT(deleteLater()));
        QString str(skt->peerAddress().toString());
        prt(info,"client %s:%d connected",str.toStdString().data(),skt->peerPort());
        ClientSession *client=new ClientSession(skt);
        connect(client,SIGNAL(socket_error(ClientSession*)),this,SLOT(delete_client(ClientSession*)));
        connect(skt,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
        clients.append(client);
    }
    void delete_client(ClientSession *c)
    {
        prt(info,"client %s disconnected",c->ip().toStdString().data());
        delete c ;
        clients.removeOne(c);
    }
    void  displayError(QAbstractSocket::SocketError socketError)
    {
        prt(info,"err when connecting to server");
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<"1";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<"1";
            break;
        default:
            qDebug()<<"1";
        }
    }

private:
    char recv_buf[Pd::BUFFER_MAX_LENGTH];
    char send_buf[Pd::BUFFER_MAX_LENGTH];
    QTcpServer *server;//server for reply all clients request ,execute client cmds,like add cam,del cam, reconfigure cam,etc..
    QList <ClientSession *> clients;//client that connected to server
};

#endif // SERVER_H
