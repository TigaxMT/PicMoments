#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

    // Verify if About dialog is running

    bool dlgExecVal()
    {
        return dlgExec;
    }

private slots:
    void on_closeBtn_clicked();

private:
    Ui::About *ui;

    bool dlgExec = false;

    // Override the reject function of QDialog, to change dlgExec value

    void reject() override;
};

#endif // ABOUT_H
