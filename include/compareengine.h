#ifndef COMPAREENGINE_H
#define COMPAREENGINE_H

#include <QObject>

class CompareEngine : public QObject
{
    Q_OBJECT
public:
    explicit CompareEngine(QObject *parent = nullptr);

signals:

};

#endif // COMPAREENGINE_H
