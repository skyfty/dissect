#ifndef MAPPINGOPTIONS_H
#define MAPPINGOPTIONS_H

#include <QObject>

class QJsonObject;

class Mapping : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(bool onlyValid READ onlyValid WRITE setOnlyValid NOTIFY onlyValidChanged FINAL)
    Q_PROPERTY(int sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged FINAL)

public:

public:
    explicit Mapping(QObject *parent = nullptr);
    ~Mapping() = default;
    void fromJson(const QJsonObject &v);
    QJsonValue toJson() const;

    bool onlyValid() const;
    void setOnlyValid(bool newOnlyValid);

    int sortRole() const;
    void setSortRole(int newSortRole);

signals:

    void onlyValidChanged();

    void sortRoleChanged();

private:
    bool m_onlyValid = true;
    int m_sortRole = 1;
};

#endif // MAPPINGOPTIONS_H
