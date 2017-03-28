﻿#include "QInteractorStyleWindowLevelThreshold.h"
#include "QInteractorStyleWindowLevelThreshold.h"
#include "ui_QInteractorStyleWindowLevelThreshold.h"
#include "ui_QAbstractNavigation.h"
#include "ImageViewer.h"

#include <vtkImageData.h>
#include <vtkObjectFactory.h>

#include <qpushbutton.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlabel.h>

vtkStandardNewMacro(QInteractorStyleWindowLevelThreshold);
QSETUP_UI_SRC(QInteractorStyleWindowLevelThreshold);
using namespace std;
QInteractorStyleWindowLevelThreshold::QInteractorStyleWindowLevelThreshold(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleWindowLevelThreshold::~QInteractorStyleWindowLevelThreshold()
{
	QDELETE_UI();
}

void QInteractorStyleWindowLevelThreshold::uniqueEnable()
{
	QAbstractNavigation::uniqueEnable();
	UpdateTargetViewer();
}

void QInteractorStyleWindowLevelThreshold::SetThresholdByViewer(int lower, int upper)
{
	InteractorStyleWindowLevelThreshold::SetThresholdByViewer(lower, upper);

	m_spinBoxLowerThreshold->setValue(lower);
	m_spinBoxUpperThreshold->setValue(upper);
}

void QInteractorStyleWindowLevelThreshold::UpdateTargetViewer()
{
	//QStringList listOfModalityName;
	ui->comboBoxTargeImage->clear();
	m_listOfModalityNames.clear();
	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleWindowLevelThreshold* _style = QInteractorStyleWindowLevelThreshold::SafeDownCast(*cit);
		// because not all have been CustomEnabled this time
		if (_style /*&& _style->GetCustomEnabled()*/) {
			m_listOfModalityNames.append(QString::fromStdString(_style->GetImageViewer()->GetWindowName()));
		}
	}
	m_listOfModalityNames.removeDuplicates();
	//if (m_listOfModalityNames != listOfModalityName)
	//{
	for (int i = 0; i < m_listOfModalityNames.size(); ++i) {
		ui->comboBoxTargeImage->addItem(m_listOfModalityNames[i]);
	}
	//}
}

void QInteractorStyleWindowLevelThreshold::initialization()
{
	m_label = new QLabel(this);
	m_spinBoxUpperThreshold = new QSpinBox(this);
	m_spinBoxLowerThreshold = new QSpinBox(this);
	m_sliderUpperThreshold = new QSlider(Qt::Horizontal, this);
	m_sliderLowerThreshold = new QSlider(Qt::Horizontal, this);
	m_pushButtonReset = new QPushButton("Reset", this);


	ui->gridLayout->addWidget(m_label, numOfMyself, 0);
	ui->gridLayout->addWidget(m_spinBoxLowerThreshold, numOfMyself, 1);
	ui->gridLayout->addWidget(m_sliderLowerThreshold, numOfMyself, 2);
	ui->gridLayout->addWidget(m_spinBoxUpperThreshold, numOfMyself, 3);
	ui->gridLayout->addWidget(m_sliderUpperThreshold, numOfMyself, 4);
	ui->gridLayout->addWidget(m_pushButtonReset, numOfMyself, 5);

	connect(m_pushButtonReset, SIGNAL(clicked()),
		this, SLOT(ResetWindowLevel()), Qt::UniqueConnection);
	connect(m_spinBoxUpperThreshold, SIGNAL(valueChanged(int)),
		this, SLOT(SetUpperThreshold(int)), Qt::UniqueConnection);
	connect(m_spinBoxLowerThreshold, SIGNAL(valueChanged(int)),
		this, SLOT(SetLowerThreshold(int)), Qt::UniqueConnection);
	connect(m_spinBoxUpperThreshold, SIGNAL(valueChanged(int)),
		m_sliderUpperThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_spinBoxLowerThreshold, SIGNAL(valueChanged(int)),
		m_sliderLowerThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderUpperThreshold, SIGNAL(valueChanged(int)),
		m_spinBoxUpperThreshold, SLOT(setValue(int)), Qt::UniqueConnection);
	connect(m_sliderLowerThreshold, SIGNAL(valueChanged(int)),
		m_spinBoxLowerThreshold, SLOT(setValue(int)), Qt::UniqueConnection);

	// set brushShape
	connect(ui->comboBoxLabel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetOutputLabel(int)));
}

void QInteractorStyleWindowLevelThreshold::uniqueInitialization()
{
	connect(ui->pushButtonThreshold, SIGNAL(clicked()),
		this, SLOT(ThresholdTargetViewerToOverlay()));
}

void QInteractorStyleWindowLevelThreshold::SetCustomEnabled(bool flag)
{
	InteractorStyleWindowLevelThreshold::SetCustomEnabled(flag);
	uniqueInvoke(flag);
	if (flag) {
		double* range = GetImageViewer()->GetInput()->GetScalarRange();
		m_label->setText(GetImageViewer()->GetWindowName());
		m_spinBoxUpperThreshold->setRange(range[0], range[1]);
		m_sliderUpperThreshold->setRange(range[0], range[1]);
		m_spinBoxLowerThreshold->setRange(range[0], range[1]);
		m_sliderLowerThreshold->setRange(range[0], range[1]);
		m_spinBoxLowerThreshold->setValue(GetLevel() - 0.5*GetWindow());
		m_spinBoxUpperThreshold->setValue(GetLevel() + 0.5*GetWindow());
	}
}

void QInteractorStyleWindowLevelThreshold::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStyleWindowLevelThreshold::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStyleWindowLevelThreshold::SetLowerThreshold(int lower)
{
	if (int(GetLevel() - GetWindow()*0.5 + 0.5) != lower) {
		//SetLowerThreshold(lower);
		if (lower > m_spinBoxUpperThreshold->value()) {
			SetThreshold(lower, lower);
		}
		else {
			SetThreshold(lower, m_spinBoxUpperThreshold->value());
		}
	}
}

void QInteractorStyleWindowLevelThreshold::SetUpperThreshold(int upper)
{
	if (int(GetLevel() + GetWindow()*0.5 + 0.5) != upper) {
		if (upper < m_spinBoxLowerThreshold->value()) {
			SetThreshold(upper, upper);
		}
		else {
			SetThreshold(m_spinBoxLowerThreshold->value(), upper);
		}
		//SetUpperThreshold(upper);
	}
}

void QInteractorStyleWindowLevelThreshold::ThresholdTargetViewerToOverlay()
{

	for (list<AbstractInteractorStyleImage*>::const_iterator cit = m_imageStyles.cbegin();
		cit != m_imageStyles.cend(); ++cit) {
		QInteractorStyleWindowLevelThreshold* _style = QInteractorStyleWindowLevelThreshold::SafeDownCast(*cit);
		if (_style && _style->GetCustomEnabled() && 
			QString::fromStdString(_style->GetImageViewer()->GetWindowName()) ==
			ui->comboBoxTargeImage->currentText()) {
			_style->ThresholdToOverlay();
			break;
		}
	}
}

void QInteractorStyleWindowLevelThreshold::SetOutputLabel(int label)
{
	InteractorStyleWindowLevelThreshold::SetOutputLabel(label);
}

void QInteractorStyleWindowLevelThreshold::ResetWindowLevel()
{
	InteractorStyleWindowLevelThreshold::ResetWindowLevel();
}