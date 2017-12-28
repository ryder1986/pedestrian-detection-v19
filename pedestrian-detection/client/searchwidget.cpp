#include "searchwidget.h"
#include "tool1.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchWidget)
{
    num=0;
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setWindowTitle(QString("server ip"));
    ui->tableWidget->setColumnWidth(0,300);
}

SearchWidget::~SearchWidget()
{
    delete ui;
}

void SearchWidget::on_tableWidget_doubleClicked(const QModelIndex &index)
{

   int row=index.row();
   QTableWidgetItem *i=ui->tableWidget->item(row,0);
   prt(info,"%s",i->text().toStdString().data());
   emit select_ip(i->text());
   this->hide();
}
