#ifndef PROCESSEDDATASENDER_H
#define PROCESSEDDATASENDER_H


#include <QObject>
#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include <QNetworkInterface>
#include "tool1.h"
#include "pd.h"

class ProcessedDataSender : public QObject{
    Q_OBJECT

public:
//    static ProcessedDataSender &get_instance()
//    {
//        static ProcessedDataSender sender;
//        return sender;
//    }
    static ProcessedDataSender *get_instance()
    {
        static ProcessedDataSender sender;
        return &sender;
    }
    void send(QByteArray datagram,const QHostAddress addr)
    {
//        QByteArray datagram;
//        datagram.clear();
//        datagram.append("test");
        prt(debug,"broadcast %s to port 12346",datagram.data());
        udp_skt->writeDatagram(datagram.data(), datagram.size(),
                               addr, Protocol::SERVER_DATA_OUTPUT_PORT);
    }
private:
    ProcessedDataSender(){
     //   timer=new QTimer();
      //  connect(timer,SIGNAL(timeout()),this,SLOT(check_client()));//TODO:maybe replace with readReady signal
        udp_skt = new QUdpSocket();
     //   udp_skt->bind(Protocol::SERVER_DATA_OUTPUT_PORT,QUdpSocket::ShareAddress);
        // timer->start(1000);
    }
    ~ProcessedDataSender()
    {
     //   disconnect(timer);
      //  delete timer;
        delete udp_skt;
    }

//    void start()
//    {
//        timer->start(1000);
//    }

//    void stop()
//    {
//        timer->stop();
//    }



public  slots:
    void check_client()
    {
        //   prt(info,"checking client");
        QByteArray client_msg;
        char *msg;
        if(udp_skt->hasPendingDatagrams())
        {
            client_msg.resize((udp_skt->pendingDatagramSize()));
            udp_skt->readDatagram(client_msg.data(),client_msg.size());
            prt(info,"msg :%s",msg=client_msg.data());
            if(!strcmp(msg,"pedestrian"))
                send_buffer_to_client();
            //   udp_skt->flush();
        }else{
            //prt(debug,"searching client on port %d",Protocol::SERVER_REPORTER_PORT)
        }
    }

    void send_buffer_to_client()
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
        udp_skt->writeDatagram(datagram.data(), datagram.size(),
                               QHostAddress::Broadcast, Protocol::CLIENT_REPORTER_PORT);
    }
private:
    QTimer *timer;
    QUdpSocket *udp_skt;
};


#endif // PROCESSEDDATASENDER_H
