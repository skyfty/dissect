#ifndef LAYOUTSAVERINSTANTIATOR_H
#define LAYOUTSAVERINSTANTIATOR_H

#include <QObject>
#include <QPointer>

#include <kddockwidgets/LayoutSaver.h>

class Profile;

class LayoutSaver : public QObject, public KDDockWidgets::LayoutSaver
{
    Q_OBJECT
    Q_PROPERTY(Profile* profile WRITE setProfile FINAL);

public:
    explicit LayoutSaver(QObject *parent = nullptr);
    ~LayoutSaver() override;
    void setProfile(Profile* profile);

    Q_INVOKABLE bool setLayout(const QString &fileName);
    Q_INVOKABLE bool save();
    Q_INVOKABLE bool restore();


private:
    void restoreDefaultLayoutFile(const QString &layoutFilePath);

protected:
    QPointer<Profile> m_profile;
};

#endif // LAYOUTSAVERINSTANTIATOR_H
