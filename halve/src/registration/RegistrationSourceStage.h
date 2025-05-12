#pragma once

#include <QSharedPointer>
#include <QVariantList>
#include "RegistrationStage.h"

class CtDataStorage;
class RegistrationSourceStage : public RegistrationStage {
	Q_OBJECT;
	Q_PROPERTY(QVariantList visibleStates READ visibleStates WRITE setVisibleStates NOTIFY visibleStatesChanged);
	Q_PROPERTY(CtDataStorage* ctDataStorage READ ctDataStorage WRITE setCtDataStorage);
public:
	explicit RegistrationSourceStage(QQuickItem* parent = nullptr);
	~RegistrationSourceStage() override = default;
	Q_INVOKABLE void loadCtData(const QString& dicomDir);
private:
	void showNifti(const QString& niftiPath);
	void clearAllModels();
	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
	void updateVisibleStates();
public:
	QVariantList visibleStates() const;
	void setVisibleStates(const QVariantList& states);
	CtDataStorage* ctDataStorage() const;
	void setCtDataStorage(CtDataStorage* ctDataStorage);
signals:
	void visibleStatesChanged();
private:
	QVariantList m_visibleStates;
	CtDataStorage* m_ctDataStorage{ nullptr };
};
