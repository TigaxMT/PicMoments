#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    dlgExec = true;

    this->setFixedSize(526,222);
}

About::~About()
{
    delete ui;
}

void About::reject()
{
    dlgExec = false;

    QDialog::reject();
}

void About::on_closeBtn_clicked()
{
    this->close();
}
