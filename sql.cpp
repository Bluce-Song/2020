#include "sql.h"
#include "ui_sql.h"

SQL::SQL(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SQL)
{
    this->setWindowFlags(Qt::FramelessWindowHint);  //去掉标题栏
    ui->setupUi(this);

    QDir dir;
    if (!dir.exists("./nandflash/ai.db")) {
        QFile file("./nandflash/ai.db");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.close();
    }

    system("chmod +x ./nandflash/ai.db");

    QSqlDatabase aip = QSqlDatabase::addDatabase("QSQLITE", "ai");
    aip.setDatabaseName("./nandflash/ai.db");
    if (aip.open()) {
        //
    } else {
        qDebug() << "SqlDesktop::initSql() sqlite:" << aip.lastError();
    }

    backupSqlRecord();
    createSqlRecord();


//    QPalette pll = ui->tableView->palette();
//    pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
//    ui->tableView->setPalette(pll);


    ui->tableView->horizontalScrollBar()->setStyleSheet("QScrollBar{background:gray; height:20px;}"
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
     "QScrollBar::handle:hover{background:gray;}"
     "QScrollBar::sub-line{background:transparent;}"
     "QScrollBar::add-line{background:transparent;}");
    ui->tableView->verticalScrollBar()->setStyleSheet("QScrollBar{background:gray; width: 20px;}"
     "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
     "QScrollBar::handle:hover{background:gray;}"
     "QScrollBar::sub-line{background:transparent;}"
     "QScrollBar::add-line{background:transparent;}");
    model = new QSqlTableModel(this, aip);
    model->setTable("aip_record");
    model->setSort(2, Qt::DescendingOrder);
    model->select();
    // 设置编辑策略
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

SQL::~SQL()
{
    delete ui;
}

void SQL::backupSqlRecord()
{
#ifdef __arm__
    QProcess cmd;
    cmd.start("du -s /mnt/nandflash/ai.db");
    cmd.waitForFinished();
    int ss = 0;
    QByteArray df = cmd.readAll();
    QString sf = df.mid(0, df.indexOf("\t"));
    ss = sf.toInt();
    if (ss > 20000) {
        QString w = tr("当前存储的数据量较多,正在备份到SD卡...");
        t.restart();
        box = new QMessageBox(this);
        box->setText(w);
        box->show();
        // wait
        QElapsedTimer t;
        t.start();
        while (t.elapsed() < 10)
            QCoreApplication::processEvents();
        updateSqlMessage();
    }
#endif
}


void SQL::updateSqlMessage()
{
    QProcess cmd;
    QString name = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    if (existsSdcardDisk()) {
        QString p = "mv /mnt/nandflash/ai.db /mnt/sdcard/";
        p.append(name + ".db");
        cmd.start(p);
        cmd.waitForFinished();
    } else {
        QString w = tr("未发现SD卡,点击OK后清除数据");
        QMessageBox::warning(this, "", w, QMessageBox::Ok);
        qDebug() << "SqlDesktop::updateSqlMessage()" << system("rm /mnt/nandflash/ai.db");
    }
    qDebug() << "SqlDesktop::updateSqlMessage()" << system("reboot");
}

bool SQL::existsSdcardDisk()
{
    bool isExist = false;
    QProcess cmddf;
    cmddf.start("df -h");
    if (cmddf.waitForFinished()) {
        QByteArray bytedf = cmddf.readAll();
        QStringList listdf = QString(bytedf).split("\n");
        for (int i=0; i < listdf.size(); i++) {
            if (listdf.at(i).startsWith("/dev/mmcblk0p1")) {
                isExist =  true;
                break;
            } else {
                //
            }
        }
    } else {
        //
    }
    cmddf.deleteLater();
    return isExist;
}

void SQL::createSqlRecord()
{
    QSqlQuery query(QSqlDatabase::database("ai"));
    QString cmd;
    cmd = "create table if not exists aip_record (";
    cmd += "id primary key, date text, time text,";
    cmd += "model text,judge text,user text,";
    cmd += "temp  text,station text,number text,";
    cmd += "item text,";
    cmd += "item1_param text, item1_result text, item1_judge text,";
    cmd += "item2_param text, item2_result text, item2_judge text,";
    cmd += "item3_param text, item3_result text, item3_judge text,";
    cmd += "item4_param text, item4_result text, item4_judge text,";
    cmd += "item5_param text, item5_result text, item5_judge text,";
    cmd += "item6_param text, item6_result text, item6_judge text,";
    cmd += "item7_param text, item7_result text, item7_judge text,";
    cmd += "item8_param text, item8_result text, item8_judge text)";
    if (query.exec(cmd)) {
        //
    } else {
        qDebug() << "SqlDesktop::createSqlRecord()" << query.lastError();
    }
}

void SQL::on_pushButton_clicked()
{
    int i;
    QString ai_date;
    QString ai_time;
    int id;
    ai_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    ai_time = QDateTime::currentDateTime().toString("hh:mm:ss");

    id = selectMax("aip_record");
    qDebug() << id;
    QSqlQuery query(QSqlDatabase::database("ai"));
    QSqlDatabase::database("ai").transaction();

    query.prepare("insert into aip_record values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(id + 1);
    query.addBindValue(ai_date); // 日期
    query.addBindValue(ai_time); // 時間
    query.addBindValue(id + 2); // 电机型号
    query.addBindValue(id + 3); // 判定
    query.addBindValue(id + 4); // 用户
    query.addBindValue(id + 5); // 温度
    query.addBindValue(id + 6); // 工位
    query.addBindValue(id + 7); // 编码
    query.addBindValue(id + 8); // 项目

    for (i = 0; i < 24; i++) {
        query.addBindValue(id + 9 + i); // 项目
    }
    if (!query.exec()) {
        qDebug() << "SqlDesktop::recvSqlDat  insert error" << query.lastError();
    }

    QSqlDatabase::database("ai").commit();
}

int SQL::selectMax(QVariant table)
{
    int max_id = 0;
    QSqlQuery query(QSqlDatabase::database("ai"));
    QString str = "select max(id) from :table";
    str.replace(":table", table.toString());
    if (!query.exec(str)) {
        qDebug() << query.lastError();
    } else {
        while (query.next()) {
            max_id = query.value(0).toInt();
        }
    }
    return max_id;
}

void SQL::on_pushButton_2_clicked()
{
    int i;
    int x;
    QString ai_date;
    QString ai_time;
    int id;
    for (x = 0; x < 10000; x++) {
        ai_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
        ai_time = QDateTime::currentDateTime().toString("hh:mm:ss");

        id = selectMax("aip_record");
        qDebug() << id;
        QSqlQuery query(QSqlDatabase::database("ai"));
        QSqlDatabase::database("ai").transaction();

        query.prepare("insert into aip_record values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
        query.addBindValue(id + 1);
        query.addBindValue(ai_date); // 日期
        query.addBindValue(ai_time); // 時間
        query.addBindValue(id + 2); // 电机型号
        query.addBindValue(id + 3); // 判定
        query.addBindValue(id + 4); // 用户
        query.addBindValue(id + 5); // 温度
        query.addBindValue(id + 6); // 工位
        query.addBindValue(id + 7); // 编码
        query.addBindValue(id + 8); // 项目

        for (i = 0; i < 24; i++) {
            query.addBindValue(id + 9 + i); // 项目
        }
        if (!query.exec()) {
            qDebug() << "SqlDesktop::recvSqlDat  insert error" << query.lastError();
        }

        QSqlDatabase::database("ai").commit();
    }
}

void SQL::on_Key_A_clicked()
{
    QString startDateTime;
    QString endDateTime;
    startDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    endDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString SqlStr = QString("date >= '%1' and date <= '%2'").arg(startDateTime).arg(endDateTime);
    model->setFilter(SqlStr);
    model->select();
    ui->tableView->hideColumn(0);
}

void SQL::on_Key_C_clicked()
{
    //
}
