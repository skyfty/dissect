#ifndef MAPPINGOPTIONS_H
#define MAPPINGOPTIONS_H

#include <QObject>

class QJsonObject;

class Mapping : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(bool onlyValid READ onlyValid WRITE setOnlyValid NOTIFY onlyValidChanged FINAL)
    Q_PROPERTY(int sortRole READ sortRole WRITE setSortRole NOTIFY sortRoleChanged FINAL)

    Q_PROPERTY(bool showRepeatInvalid READ showRepeatInvalid WRITE setShowRepeatInvalid NOTIFY showRepeatInvalidChanged FINAL)
    Q_PROPERTY(bool showMappingInvalid READ showMappingInvalid WRITE setShowMappingInvalid NOTIFY showMappingInvalidChanged FINAL)
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

    bool showRepeatInvalid() const;
    void setShowRepeatInvalid(bool newShowRepeatInvalid);

    bool showMappingInvalid() const;
    void setShowMappingInvalid(bool newShowMappingInvalid);

signals:

    void onlyValidChanged();

    void sortRoleChanged();

    void showRepeatInvalidChanged();

    void showMappingInvalidChanged();

private:
    bool m_onlyValid = true;
    bool m_showRepeatInvalid = false;
    bool m_showMappingInvalid = false;
    int m_sortRole = 1;
};

#endif // MAPPINGOPTIONS_H
