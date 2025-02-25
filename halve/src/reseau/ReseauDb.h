#ifndef RESEAUDB_H
#define RESEAUDB_H

#include "utility/VtkDataWarp.h"

class Reseau;
class vtkPolyData;
class vtkIdList;
class vtkVector3d;

class ReseauDb : public QObject
{
    Q_OBJECT
public:
    ReseauDb(const QString &path, QObject *parent = nullptr);
    bool save();
    void load();
    void backup(const QString &path);

    void remove(Reseau* reseau);
    void remove(int row);
    void clean();
    Reseau *add(qint64 id, const QString &name);
    Reseau* add(qint64 id, const QString& name, const QColor& color, vtkIdList *ids, vtkSmartPointer<vtkPolyData> polyData);

    int size() const;
    Reseau* getData(int row) const;
    Reseau* getData(qint64 id) const;

    QList<QPair<Reseau*, double>> getDatas(double x, double y, double z) const;
    QList<QPair<Reseau*, double>> getDatas(double x[3]) const;
    QList<QPair<Reseau*, double>> getDatas(const vtkVector3d &pos) const;

    QList<Reseau *> getDatas() const;
    QList<Reseau *> getApparentDatas() const;

    int index(const Reseau *reseau) const;
    int index(qint64 id) const;
    int indexOfName(const QString &name) const;


signals:
    void added(Reseau* reseau);
    void deleted(Reseau* reseau);
    void cleaned();
    void changed(Reseau* reseau);
    void apparentChanged(Reseau* reseau);
    void dyestuffChanged(Reseau* reseau);

private:
    void load(const QByteArray &catheterJson);
    Reseau* load(const QJsonObject &json);
    void connectSignals(Reseau *reseau);
    QJsonObject loadDefaultReseau();
    QString getStoragePath(const QString &path) const;
    QString getReseausStoragePath(const QString &path) const;


private:
    QString m_path;
    QString m_reseauPath;
    QList<Reseau*> m_reseaus;
};

#endif // RESEAUDB_H
