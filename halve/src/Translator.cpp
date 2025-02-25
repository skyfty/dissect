#include "Translator.h"

#include <QCoreApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>

Translator::Translator(const QString &language,QQmlApplicationEngine *parent)
    : QObject{parent},m_engine(parent)
{
    setLanguage(language);
}

void Translator::setLanguage(const QString &language) {
    if (m_translator != nullptr) {
        QCoreApplication::removeTranslator(m_translator);
        delete m_translator;
    }
    m_translator = new QTranslator(this);
    if (m_translator->load(language,":/i18n")) {
        QCoreApplication::installTranslator(m_translator);
    }
}

void Translator::reload(const QString &language) {
    setLanguage(language);
    m_engine->retranslate();
}
