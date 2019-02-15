#ifndef SQL_H
#define SQL_H

#include <QtSql>
#include <QWidget>
#include <QFile>
#include <QDir>
#include <QSqlDatabase>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QStackedWidget>
#include <QSqlError>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QScrollBar>

namespace Ui {
class SQL;
}

class SQL : public QWidget
{
    Q_OBJECT
    
public:
    explicit SQL(QWidget *parent = 0);
    ~SQL();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Key_A_clicked();

    void on_Key_C_clicked();

private:
    Ui::SQL *ui;

    QElapsedTimer t;
    QMessageBox *box;
    QSqlTableModel *model;

    int selectMax(QVariant table);

    void backupSqlRecord();
    void updateSqlMessage();
    bool existsSdcardDisk();
    void createSqlRecord();
};

#endif // SQL_H
