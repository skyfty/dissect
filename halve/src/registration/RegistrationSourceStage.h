#pragma once

#include <QSharedPointer>
#include <QVariantList>
#include "RegistrationStage.h"

class RegistrationSourceStage : public RegistrationStage {
	Q_OBJECT;
	Q_PROPERTY(QVariantList visibleStates READ visibleStates WRITE setVisibleStates NOTIFY visibleStatesChanged)
public:
	explicit RegistrationSourceStage(QQuickItem* parent = nullptr);
	~RegistrationSourceStage() override = default;
	void showNifti(const QString& niftiPath);
	Q_INVOKABLE void loadCtData(const QString& dicomDir);
private:
	vtkUserData initializeVTK(vtkRenderWindow* renderWindow) override;
	void destroyingVTK(vtkRenderWindow* renderWindow, vtkUserData userData) override;
public:
	QVariantList visibleStates() const;
	void setVisibleStates(const QVariantList& states);

signals:
	void visibleStatesChanged();
private:
	QVariantList m_visibleStates;
};
