#include <QApplication>
#include <QMessageBox>
#include <QProcess>
#include <QFile>

int main(int argc, char *argv[])
{
    //Put environment variables
    qputenv("XDG_CURRENT_DESKTOP", "KDE");
    qputenv("DE", "kde");
    qputenv("KDE_SESSION_VERSION", "5");
    qputenv("QT_QPA_PLATFORMTHEME", "ts");

    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);

    bool blueprint = false;
    if (a.arguments().contains("--blueprint")) {
        blueprint = true;
    }

    QProcess* tsProcess = new QProcess();
    if (blueprint) {
        if (QFile("/usr/bin/theshellb").exists()) {
            tsProcess->start("theshellb");
        } else {
            QMessageBox::warning(NULL, "theShell Blueprint", "theShell Blueprint was not found on this computer.", QMessageBox::Ok, QMessageBox::Ok);
            return 1;
        }
    } else {
        tsProcess->start("theshell");
    }

    QObject::connect(tsProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
        [=](int exitCode, QProcess::ExitStatus exitStatus){
        if (exitCode == 0) {
            QCoreApplication::exit(0);
        } else {
            //Restart theShell
            if (QMessageBox::question(NULL, "theShell crashed!", "theShell has crashed. Do you want to restart theShell?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
                if (blueprint) {
                    if (QFile("/usr/bin/theshellb").exists()) {
                        tsProcess->start("theshellb");
                    } else {
                        QMessageBox::warning(NULL, "theShell Blueprint", "theShell Blueprint was not found on this computer.", QMessageBox::Ok, QMessageBox::Ok);
                        QApplication::exit(1);
                    }
                } else {
                    tsProcess->start("theshell");
                }
            } else {
                QCoreApplication::exit(0);
            }
        }
    });

    return a.exec();
}
