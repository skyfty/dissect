#pragma once
#include <QVariantList>
#include <vtkSmartPointer.h>
#include <QList>

class Profile;
class Obscurity;
class vtkPoints;
class vtkPolyData;
class RegistrationProcessor : public QObject
{
	Q_OBJECT;
	Q_PROPERTY(int undoType READ undoType WRITE setUndoType FINAL);
	Q_PROPERTY(bool enableUndo READ enableUndo NOTIFY enableUndoChanged);
	Q_PROPERTY(bool enableRegistration READ enableRegistration NOTIFY enableRegistrationChanged);
	Q_PROPERTY(bool enablePick READ enablePick WRITE setEnablePick NOTIFY enablePickChanged FINAL);
public:
	explicit RegistrationProcessor(QObject* parent = nullptr);

	Q_INVOKABLE bool startRegistration(Profile* profile, Obscurity* obscurity, QVariantList visibleStates);
	/// @brief 获取当前撤销操作类型（0=无操作，1=源点，-1=目标点）
	int undoType() const;
	void setUndoType(int undoType);
	/// @brief 执行撤销操作（删除最近添加的配准点,根据undoType确定是源点还是目标点）
	Q_INVOKABLE void undo();
	void clearUndoStack();
	/// @brief 检查撤销功能是否可用,决定qml中"撤销"按钮的enabled状态
	bool enableUndo() const;
	/// @brief 检查配准条件是否满足(需至少存在一对配准点),决定qml中"开始配准"按钮的enabled状态
	bool enableRegistration() const;
	bool enablePick() const;
	void setEnablePick(bool enablePick);
	void reset();
public:
	/// @brief 计算需要描边的点的索引。
	/// @detail 为了突出显示配准时，左右两侧的当前配准点，需要将当前的配准点描边显示。
	/// 根据m_sourcePoints和m_targetPoints的size进行计算，如果size相同，返回-1，表示无需描边；否则返回对应的索引
	/// @return 返回配准点的索引。
	vtkIdType calcOutlinePointIndex() const;
signals:
	void enableUndoChanged();
	void enableRegistrationChanged();
	void enablePickChanged();
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
	bool m_enableUndo{ false };
	bool m_enablePick{ false };
};