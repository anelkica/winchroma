#ifndef WINDOW_EFFECTS_H
#define WINDOW_EFFECTS_H

#include <QObject>
#include <QQmlEngine>

class WindowEffects : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
public:
    explicit WindowEffects(QObject *parent = nullptr);

    Q_INVOKABLE void setWindowBorder(const QString &title);
signals:
};

#endif // WINDOW_EFFECTS_H
