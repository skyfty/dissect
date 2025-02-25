#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QPointer>

class QQmlApplicationEngine;
class QTranslator;

class Translator : public QObject
{
    Q_OBJECT
public:
    Translator(const QString &language, QQmlApplicationEngine *parent = nullptr);
    Q_INVOKABLE void reload(const QString &language);

private:
    void setLanguage(const QString &language);
private:
    QQmlApplicationEngine *m_engine = nullptr;
    QPointer<QTranslator> m_translator;
};

#endif // TRANSLATOR_H
