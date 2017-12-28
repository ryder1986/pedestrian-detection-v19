#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "ui_searchwidget.h"
namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = 0);
    ~SearchWidget();
    void add_text(QString ip)
    {
        ui->tableWidget->setItem(num,0,new QTableWidgetItem(ip));
        num++;
    }
    void clear_text()
    {
         ui->tableWidget->clear();
         num=0;
//        int i=0;
//        for(i=0;i<10;i++)
//        ui->tableWidget->removeRow(i);
    }

private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);
signals:
    void select_ip(QString ip);

private:
    Ui::SearchWidget *ui;
    int num;
};

#endif // SEARCHWIDGET_H
