#pragma once

#include <QObject>

class ProgressReporter : public QObject
{
	Q_OBJECT;
	Q_PROPERTY(QString progressText READ progressText WRITE setProgressText NOTIFY progressTextChanged);
	Q_PROPERTY(double progressValue READ progressValue NOTIFY progressValueChanged);
public:
	QString progressText() const;
	void setProgressText(const QString& progressText);
	double progressValue() const;
	void setStepProgressValue(double stepProgressValue);
	/**
	 * @brief 完成当前步骤，将当前步骤权重累加至总进度
	 * @note 调用后需通过setStepWeight()设置下一步权重，否则后续进度无法更新
	 */
	void completeStep();
	/**
	 * @brief 设置当前步骤的权重占比
	 * @param stepWeight 当前步骤在总进度中的权重（总和应<=1.0）
	 */
	void setStepWeight(double stepWeight);
	void complete();
signals:
	void progressTextChanged();
	void progressValueChanged();
private:
	QString m_progressText;
	/// @brief 进度条当前的进度值(0~1)
	double m_progressValue;
	double m_beginStep{ 0.0 };
	double m_stepWeight{ 1.0 };
};