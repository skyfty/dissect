#pragma once
#include <QVariantList>
#include <vtkSmartPointer.h>
#include <QList>

class RegistrationProcessor;
class Profile;
class Obscurity;
class vtkPoints;
class vtkPolyData;
class RegistrationProcessor : public QObject
{
	Q_OBJECT;
	Q_PROPERTY(int undoType READ undoType WRITE setUndoType FINAL);
public:
	RegistrationProcessor( QObject* parent = nullptr);
	~RegistrationProcessor();
	
	Q_INVOKABLE bool startRegistration(Profile* profile, Obscurity* obscurity, QVariantList visibleStates);
	int undoType() const;
	void setUndoType(int undoType);
	Q_INVOKABLE void undo();
	void clearUndoStack();
public:
	/// @brief 计算需要描边的点的索引。
	/// @detail 为了突出显示配准时，左右两侧的当前配准点，需要将当前的配准点描边显示。
	/// 根据m_sourcePoints和m_targetPoints的size进行计算，如果size相同，返回-1，表示无需描边；否则返回对应的索引
	/// @return 返回配准点的索引。
	vtkIdType calcOutlinePointIndex() const;

private:
	void saveFusedPolyData(Profile* profile, Obscurity* obscurity, vtkSmartPointer<vtkPolyData> fusedPolyData);
public:
	vtkSmartPointer<vtkPoints> m_sourcePoints;
	vtkSmartPointer<vtkPoints> m_targetPoints;
	QList<vtkSmartPointer<vtkPolyData>> m_sourcePolyDatas;
	vtkSmartPointer<vtkPolyData> m_targetPolyData;
private:
	/// @brief 撤销哪里 source(1) or target(-1)
	QList<int> m_undoStack;
};