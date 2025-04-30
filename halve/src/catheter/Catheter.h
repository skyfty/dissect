#ifndef CATHETEROPTION_H
#define CATHETEROPTION_H

#include <QObject>
#include <QColor>
#include <vtkColor.h>
#include <vtkVector.h>
#include <QMap>
#include <QPointer>
#include "HalveType.h"
#include "catheter/CatheterMagnetism.h"
#include "catheter/CatheterMould.h"

class ElectrodeNode;
class CatheterType;
class  CatheterMesh;
class CatheterMagnetism;

class Catheter : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(QString id READ id CONSTANT FINAL);
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged);
    Q_PROPERTY(int amount READ getAmount WRITE setAmount NOTIFY amountChanged);
    Q_PROPERTY(QString manufacturer READ getManufacturer WRITE setManufacturer NOTIFY manufacturerChanged);
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged);
    Q_PROPERTY(QString version READ getVersion WRITE setVersion NOTIFY versionChanged);
    Q_PROPERTY(QList<quint16> gap READ getGap WRITE setGap NOTIFY gapChanged);
    Q_PROPERTY(QString fr READ getFr WRITE setFr NOTIFY frChanged);
    Q_PROPERTY(double diameter READ getDiameter WRITE setDiameter NOTIFY diameterChanged);
    Q_PROPERTY(double far READ getFar WRITE setFar NOTIFY farChanged);
    Q_PROPERTY(Halve::ElectrodeType electrode READ getElectrode WRITE setElectrode NOTIFY electrodeChanged);
    Q_PROPERTY(bool builtin READ getBuiltin CONSTANT);
    Q_PROPERTY(QColor dyestuff READ getDyestuff WRITE setDyestuff NOTIFY dyestuffChanged);
    Q_PROPERTY(bool multiPassEnable READ getMultiPassEnable WRITE setMultiPassEnable NOTIFY multiPassEnableChanged);
    Q_PROPERTY(double multiLowPass READ getMultiLowPass WRITE setMultiLowPass NOTIFY multiLowPassChanged);
    Q_PROPERTY(double multiHeighPass READ getMultiHeighPass WRITE setMultiHeighPass NOTIFY multiHeighPassChanged);
    Q_PROPERTY(bool singlePassEnable READ getSinglePassEnable WRITE setSinglePassEnable NOTIFY singlePassEnableChanged);
    Q_PROPERTY(double singleLowPass READ getSingleLowPass WRITE setSingleLowPass NOTIFY singleLowPassChanged);
    Q_PROPERTY(double singleHeighPass READ getSingleHeighPass WRITE setSingleHeighPass NOTIFY singleHeighPassChanged);
    Q_PROPERTY(bool showPassData READ getShowPassData WRITE setShowPassData NOTIFY showPassDataChanged);
    Q_PROPERTY(bool employ READ employ WRITE setEmploy NOTIFY employChanged FINAL)
    Q_PROPERTY(bool apparent READ apparent WRITE setApparent NOTIFY apparentChanged FINAL)
    Q_PROPERTY(double electrodeLength READ electrodeLength WRITE setElectrodeLength NOTIFY electrodeLengthChanged FINAL)
    Q_PROPERTY(bool prefab READ prefab WRITE setPrefab NOTIFY prefabChanged FINAL)
    Q_PROPERTY(bool destroyed READ destroyed WRITE setDestroyed NOTIFY destroyedChanged FINAL)
    Q_PROPERTY(bool magnetism READ magnetism WRITE setMagnetism NOTIFY magnetismChanged FINAL)
    Q_PROPERTY(CatheterMagnetism *magnetismAmbient READ catheterMagnetism CONSTANT);
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged FINAL)
    Q_PROPERTY(bool isPant READ isPant CONSTANT FINAL)
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged FINAL)
    Q_PROPERTY(quint16 centrality READ centrality WRITE setCentrality NOTIFY centralityChanged FINAL)
    Q_PROPERTY(quint16 bseat READ bseat WRITE setBseat NOTIFY bseatChanged FINAL)
    Q_PROPERTY(QString meshName READ meshName WRITE setMeshName NOTIFY meshNameChanged FINAL)

public:
    explicit Catheter(QObject *parent = nullptr);
    ~Catheter() = default;

    void fromJson(const QJsonObject &v);
    void toJson(QJsonObject &json) const;

    QString id() const {
        return m_id;
    }
    void setId(const QString &id) {
        m_id = id;
    }
    bool isBodySurface() const;
    void setName(const QString &v);
    QString getName() const;

    void setDiameter(double v);
    double getDiameter() const;


    bool getBuiltin() const {
        return m_builtin;
    }

    void setFar(double v);
    double getFar() const;

    void setAmount(int v);
    int getAmount() const;

    void setManufacturer(const QString &v);
    QString getManufacturer() const;

    void setFr(const QString &v);
    QString getFr() const;

    void setGap(const QList<quint16> &v);
    const QList<quint16>& getGap() const;

    void setGap(const QStringList &v);
    static QList<quint16> makeGap(quint32 amount);

    void setVersion(const QString &v);
    QString getVersion() const;

    void setType(const QString &v);
    QString getType() const;

    QColor getDyestuff() const;
    void setDyestuff(QColor v);
    vtkColor3ub getDyestuff3ub() const;

    bool getMultiPassEnable() const;
    void setMultiPassEnable(bool v);

    double getMultiLowPass() const;
    void setMultiLowPass(double v);

    double getMultiHeighPass() const;
    void setMultiHeighPass(double v);

    bool getSinglePassEnable() const;
    void setSinglePassEnable(bool v);

    double getSingleLowPass() const;
    void setSingleLowPass(double v);

    double getSingleHeighPass() const;
    void setSingleHeighPass(double v);

    bool getShowPassData() const;
    void setShowPassData(bool v);

    Halve::ElectrodeType getElectrode () const;
    void setElectrode(Halve::ElectrodeType v);

    bool employ() const;
    void setEmploy(bool newEmploy);

    QList<ElectrodeNode *> getElectrodeNodes(Halve::ElectrodeType electrode) const;
    QList<ElectrodeNode *> getElectrodeNodes() const;
    QList<ElectrodeNode *> getAllElectrodeNodes() const;

    ElectrodeNode* getElectrodeNode(Halve::ElectrodeType electrode, const QString &id);
    ElectrodeNode* getElectrodeNode(const QString &id);


    ElectrodeNode* getElectrodeNode(Halve::ElectrodeType electrode, quint16 d);
    ElectrodeNode* getElectrodeNode(quint16 d);

    bool isBelongTo(const ElectrodeNode *node) const;
    bool isOccupied(quint16 bseat);

    bool apparent() const;
    void setApparent(bool newApparent);

    void resetDefaultElectrodeNodes();
    void setElectrodeSeat();
    void resetElectrodeSeat();
    void resetElectrodeDyestuff(const QColor &orgDyestuff);

    double electrodeLength() const;
    void setElectrodeLength(double newElectrodeLength);

    bool prefab() const;
    void setPrefab(bool newPrefab);

    bool destroyed() const;
    void setDestroyed(bool newDestroyed);

    void setElectrodeLastPoint(const QString &id, const vtkVector3d &pos);
    vtkVector3d getElectrodeLastPoint(const QString &id);
    void clearLastPoint();

    CatheterType *catheterType() const;
    void setCatheterType(CatheterType *newCatheterType);

    CatheterMagnetism *catheterMagnetism() const;
    bool magnetism() const;
    void setMagnetism(bool newMagnetism);

    quint16 port() const;
    void setPort(quint16 newPortIdx);

    CatheterMould *catheterMould();

    void deleteMesh(const QString &meshPath);

    bool isPant() const;

    bool valid() const;
    void setValid(bool newValid);

    quint16 centrality() const;
    void setCentrality(quint16 newCentrality);

    CatheterMesh *mesh();

    quint16 bseat() const;
    void setBseat(quint16 newBseat);

    QString meshName() const;
    void setMeshName(const QString &newMeshName);

signals:
    void dyestuffChanged();
    void multiPassEnableChanged();
    void multiLowPassChanged();
    void bseatChanged();
    void multiHeighPassChanged();
    void singlePassEnableChanged();
    void singleLowPassChanged();
    void singleHeighPassChanged();
    void showPassDataChanged();
    void electrodeChanged();
    void nameChanged();
    void typeChanged();
    void magnetismChanged();
    void versionChanged();
    void frChanged();
    void gapChanged();
    void manufacturerChanged();
    void amountChanged();
    void farChanged();
    void diameterChanged();
    void employChanged();
    void electrodeNodesChanged();
    void apparentChanged();
    void electrodeLengthChanged();
    void prefabChanged();
    void destroyedChanged();
    void catheterMagnetismChanged();
    void portChanged();
    void validChanged();
    void centralityChanged();
    void mouldChanged();

    void meshNameChanged();


private:
    void resetElectrodeNames();
    QString getElectrodeName(int j) const;
    void setPairNodes(quint16 bseat);
    void setBothNodes(quint16 bseat);
    void setSingleNodes(quint16 bseat);
    ElectrodeNode *findElectrodeNode(const QList<ElectrodeNode*> &nodes, const QString &id);

private:
    QString m_id;
    QString m_name;
    int m_amount = 0;
    quint16 m_bseat = CatheterNoSeta;
    QString m_type = "ABL";
    QString m_manufacturer;
    QString m_version;
    QList<quint16> m_gap;
    QString m_fr;
    double m_diameter = 0.5;
    double m_far = 0.0;
    bool m_builtin = true;
    bool m_apparent = true;
    double m_electrodeLength = 0.3;
    bool m_prefab = false;
    bool m_employ = false;;
    bool m_destroyed = false;
    QColor m_dyestuff = Qt::white;
    bool m_multiPassEnable = false;
    double m_multiLowPass = 10.0;
    double m_multiHeighPass = 5.0;
    bool m_singlePassEnable = false;
    double m_singleLowPass = 10.0;
    double m_singleHeighPass = 5.0;
    bool m_showPassData = false;
    quint16 m_port = 0;
    bool m_isPant = false;
    QString m_meshName;
    bool m_valid = false;
    quint16 m_centrality = 0;

    Halve::ElectrodeType m_electrode = Halve::SINGLE;
    QList<ElectrodeNode*> m_pairNodes;
    QList<ElectrodeNode*> m_bothNodes;
    QList<ElectrodeNode*> m_singleNodes;
    CatheterType *m_catheterType = nullptr;
    QPointer<CatheterMagnetism> m_catheterMagnetism;
    QPointer<CatheterMould> m_catheterMould;
    QPointer<CatheterMesh> m_mesh;
    std::unique_ptr<double[]> m_detectorEntryPoint;
    QMap<QString, vtkVector3d> m_electrodeLastPos;
};

#endif // CATHETEROPTION_H
