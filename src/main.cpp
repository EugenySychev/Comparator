#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickImageProvider>
#include <QStyle>
#include "compareengine.h"

/// Attemp to get default images for icons
/// from some standart assets
//class StandardIconProvider : public QQuickImageProvider
//{
//public:
//    StandardIconProvider(QStyle *style)
//        : QQuickImageProvider(Pixmap)
//        , m_style(style)
//    {}

//    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override
//    {
//        Q_UNUSED(size)
//        static const auto metaobject = QMetaEnum::fromType<QStyle::StandardPixmap>();
//        const int value = metaobject.keyToValue(id.toLatin1());
//        QIcon icon = m_style->standardIcon(static_cast<QStyle::StandardPixmap>(value));
//        return icon.pixmap(requestedSize);
//    }

//    QStyle *m_style;
//};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setOrganizationName("Sychev");
    app.setOrganizationDomain("test");
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    // class StandartIconProvider above
    // engine.addImageProvider(QLatin1String("standardicons"), new StandardIconProvider(app.style()));
    qmlRegisterType<CompareEngine>("Engine", 1, 0, "CompareEngine");

    auto compareEngine = new CompareEngine();
    engine.rootContext()->setContextProperty("engine", compareEngine);
    engine.load(url);
    compareEngine->updateModel();
    return app.exec();
}
