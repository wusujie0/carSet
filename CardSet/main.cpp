#include <QApplication>
#include <VPApplication>

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "account.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VPApplication vplay;

    // Use platform-specific fonts instead of V-Play's default font
    vplay.setPreservePlatformFonts(true);


    qmlRegisterType<Account>("Account",1,0,"Account");
    qmlRegisterType<Card2>("Card2",1,0,"Card2");


    QQmlApplicationEngine engine;
    Account account;
    engine.rootContext()->setContextProperty("account",&account);
    vplay.initialize(&engine);

    // use this during development
    // for PUBLISHING, use the entry point below
    vplay.setMainQmlFileName(QStringLiteral("qml/widgets/Main.qml"));//qml/widgets/

    // use this instead of the above call to avoid deployment of the qml files and compile them into the binary with qt's resource system qrc
    // this is the preferred deployment option for publishing games to the app stores, because then your qml files and js files are protected
    // to avoid deployment of your qml files and images, also comment the DEPLOYMENTFOLDERS command in the .pro file
    // also see the .pro file for more details
    // vplay.setMainQmlFileName(QStringLiteral("qrc:/qml/Main.qml"));

    engine.load(QUrl(vplay.mainQmlFileName()));

    return app.exec();
}
