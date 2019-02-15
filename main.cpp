#include "sql.h"
#include <QApplication>

#define _TIME_ qPrintable(QTime::currentTime ().toString ("hh:mm:ss:zzz"))

void LogOutPut(QtMsgType type, const char* msg)
{
    QString qstrText;
    switch (type)
    {
    case QtDebugMsg:
        qstrText = QString("%1: %2").arg(_TIME_, msg);
        break;
    case QtWarningMsg:
        qstrText = QString("%1: %2").arg(_TIME_, msg);
        break;
    case QtCriticalMsg:
        qstrText = QString("%1: %2").arg(_TIME_, msg);
        break;
    case QtFatalMsg:
        qstrText = QString("%1: %2").arg(_TIME_, msg);
        break;
    }
    QFile out("log.txt");
    out.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&out);
    ts<<qstrText<<endl;
}

int main(int argc, char *argv[])
{
    qInstallMsgHandler(LogOutPut);
    QApplication a(argc, argv);
    SQL w;
    w.show();
    
    return a.exec();
}
