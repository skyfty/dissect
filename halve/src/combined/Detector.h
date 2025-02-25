#pragma once

#include <QObject>

class Detector : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(uint16_t handle MEMBER m_toolHandle CONSTANT);
    Q_PROPERTY(QString info MEMBER m_toolInfo CONSTANT);
    Q_PROPERTY(uint16_t systemStatus MEMBER m_systemStatus CONSTANT);
    Q_PROPERTY(uint32_t portStatus MEMBER m_portStatus CONSTANT);

public:
    typedef QList<QSharedPointer<Detector>> List;

public:
    explicit Detector(QObject *parent = nullptr);
    ~Detector();

public:
    //! The handle that uniquely identifies the tool
    uint16_t m_toolHandle;

    //! The TransformStatus as a two byte integer. See the related enum for its interpretation.
    uint16_t m_status;

    //! This member is useful for holding static tool information during CSV output
    QString m_toolInfo;
    //! The status of the measurement device itself
    uint16_t m_systemStatus;

    //! The status of the tool
    uint32_t m_portStatus;
};
