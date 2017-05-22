#include "QInteractorStyleSurfaceCenterLineVBDUboguStandard.h"
#include "ui_QInteractorStyleSurfaceCenterLineVBDUboguStandard.h"

#include "PerpendicularMeasurementLineWidget.h"
#include "CenterlineSurfaceViewer.h"

#include <vtkPolygonalSurfacePointPlacer.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkDistanceRepresentation.h>
#include <vtkDistanceWidget.h>
#include <vtkLineRepresentation.h>
#include <vtkRenderer.h>
#include <vtkTextActor.h>
#include <vtkHandleWidget.h>
#include <vtkCallbackCommand.h>
#include <vtkKdTreePointLocator.h>

vtkStandardNewMacro(QInteractorStyleSurfaceCenterLineVBDUboguStandard);
QSETUP_UI_SRC(QInteractorStyleSurfaceCenterLineVBDUboguStandard);

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::SetCustomEnabled(bool flag)
{
	InteractorStyleSurfaceCenterLineSimpleClipping::SetCustomEnabled(flag);
	setCurrentMode(m_currentMode);
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::setCurrentMode(int mode)
{
	if (mode < 0 || mode > 7) {
		return;
	}
	m_currentMode = mode;
	switch (m_currentMode)
	{
	case LENGTH_OF_BASILAR_ARTERY:
	case LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY:
	case LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY:
	case MINIMUM_DIAMETER_OF_BASILAR_ARTERY:
	case MINIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY:
	case MINIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY:
		if (m_lineWidget) {
			m_lineWidget->SetInteractor(nullptr);
			m_lineWidget->EnabledOff();
			m_lineWidget = nullptr;
		}
		InitializeHandleWidgets();
		GetSurfaceViewer()->GetRenderer()->AddActor(m_measurementText);
		break;
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT:
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT:
		for (int i = 0; i < NUM_OF_HANDLES; ++i) {
			// removing widgets
			if (m_handleWidgets[i]) {
				m_handleWidgets[i]->SetInteractor(nullptr);
				m_handleWidgets[i]->EnabledOff();
				m_handleWidgets[i]->RemoveAllObservers();
				m_handleWidgets[i] = nullptr;
			}

		}
		m_pointLocator = nullptr;
		GetSurfaceViewer()->GetRenderer()->RemoveActor(m_measurementText);
		InitializeLinePerpendicularMeasurementWidget();
	default:
		break;
	}
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::FindMaximumRadius()
{
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::FindMaximumRadius();
	switch (m_currentMode)
	{
	case LENGTH_OF_BASILAR_ARTERY:
		ui->doubleSpinBoxLengthOfBasilaarArtery->setValue(m_distance);
		emit valueChanged(LENGTH_OF_BASILAR_ARTERY, m_distance);
		break;
	case LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxLengthOfIntracranialSegmentOfLeftVetrabralArtery->setValue(m_distance);
		emit valueChanged(LENGTH_OF_INTRACRANIAL_SEGMENT_OF_LEFT_VERTEBRAL_ARTERY, m_distance);
		break;
	case LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxLengthOfIntracranialSegmentOfRightVetrabralArtery->setValue(m_distance);
		emit valueChanged(LENGTH_OF_INTRACRAINAL_SEGMENT_OF_RIGHT_VERTEBRAL_ARTERY, m_distance);
		break;
	case MINIMUM_DIAMETER_OF_BASILAR_ARTERY:
		ui->doubleSpinBoxMinimumDiameterOfBasilarArtery->setValue(m_minRadius * 2);
		emit valueChanged(MINIMUM_DIAMETER_OF_BASILAR_ARTERY, m_distance);
		break;
	case MINIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxMinimumDiameterOfLeftVetebralArtery->setValue(m_minRadius * 2);
		emit valueChanged(MINIMUM_DIAMETER_OF_LEFT_VERTEBRAL_ARTERY, m_distance);
		break;
	case MINIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY:
		ui->doubleSpinBoxMinimumDiameterOfRightVetebralArtery->setValue(m_minRadius * 2);
		emit valueChanged(MINIMUM_DIAMETER_OF_RIGHT_VERTEBRAL_ARTERY, m_distance);
		break;
	default:
		break;
	}
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::FindMaxiMumPerpendicularDistance()
{
	double distance = m_lineWidget->GetDistanceWidget()->GetDistanceRepresentation()->GetDistance();

	switch (m_currentMode)
	{
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT:
		ui->doubleSpinBoxDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeftIntracranialSegmentOfVertebralArteryAndBasilarArtery->setValue(distance);
		emit valueChanged(DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_LEFT, distance);
		break;
	case DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT:
		ui->doubleSpinBoxDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRightIntracranialSegmentOfVertebralArteryAndBasilarArtery->setValue(distance);
		emit valueChanged(DISTANCE_BETWEEN_CENTERLINE_AND_CONNECTION_LEAD_OF_2_TERMINALS_RIGHT, distance);
		break;
	default:
		break;
	}

}

QInteractorStyleSurfaceCenterLineVBDUboguStandard::QInteractorStyleSurfaceCenterLineVBDUboguStandard(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleSurfaceCenterLineVBDUboguStandard::~QInteractorStyleSurfaceCenterLineVBDUboguStandard()
{
	QDELETE_UI();
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::InitializeLinePerpendicularMeasurementWidget()
{
	if (!GetCenterlineSurfaceViewer()->GetCenterline() ||
		GetCenterlineSurfaceViewer()->GetCenterline()->GetNumberOfPoints() < 1) {
		vtkErrorMacro(<< "no centerline ");
		return;
	}
	vtkSmartPointer<vtkCallbackCommand> _callback =
		vtkSmartPointer<vtkCallbackCommand>::New();
	_callback->SetClientData(this);
	_callback->SetCallback( [](vtkObject *caller, unsigned long eid,
		void *clientdata, void *calldata) {
		QInteractorStyleSurfaceCenterLineVBDUboguStandard* _self =
			reinterpret_cast<QInteractorStyleSurfaceCenterLineVBDUboguStandard*>(clientdata);
		_self->FindMaxiMumPerpendicularDistance();
	});
	

	vtkSmartPointer<vtkPolygonalSurfacePointPlacer> pointPlacer =
		vtkSmartPointer<vtkPolygonalSurfacePointPlacer>::New();
	pointPlacer->AddProp(GetCenterlineSurfaceViewer()->GetCenterlineActor());


	m_lineWidget = vtkSmartPointer<PerpendicularMeasurementLineWidget>::New();
	m_lineWidget->AddObserver(vtkCommand::InteractionEvent, _callback);
	m_lineWidget->SetInteractor(this->Interactor);
	m_lineWidget->CreateDefaultRepresentation();
	m_lineWidget->GetLineRepresentation()->GetPoint1Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget->GetLineRepresentation()->GetPoint2Representation()->SetPointPlacer(pointPlacer);
	m_lineWidget->GetLineRepresentation()->SetPoint1WorldPosition(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(0));
	m_lineWidget->GetLineRepresentation()->SetPoint2WorldPosition(
		GetCenterlineSurfaceViewer()->GetCenterline()->GetPoint(1));
	m_lineWidget->GetLineRepresentation()->SetDistanceAnnotationFormat("%-#11.2f mm");
	m_lineWidget->GetDistanceWidget()->GetDistanceRepresentation()->SetLabelFormat("%-#11.2f mm");
	m_lineWidget->SetLine(GetCenterlineSurfaceViewer()->GetCenterline());
	m_lineWidget->EnabledOn();
}

void QInteractorStyleSurfaceCenterLineVBDUboguStandard::initialization()
{
	connect(ui->pushButtonLengthOfBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToLengthOfBasilarArtery()));
	connect(ui->pushButtonLengthOfIntracranialSegmentOfLeftVetrabralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToLengthOfIntracranialSegmentOfLeftVertebralArtery()));
	connect(ui->pushButtonLengthOfIntracranialSegmentOfRightVetrabralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToLengthOfIntracranialSegmentOfRightVertebralArtery()));
	connect(ui->pushButtonDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeftIntracranialSegmentOfVertebralArteryAndBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsLeft()));
	connect(ui->pushButtonDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRightIntracranialSegmentOfVertebralArteryAndBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToDistanceBetweenCenterlineAndConnectionLeadOf2TerminalsRight()));
	connect(ui->pushButtonMinimumDiameterOfBasilarArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToMinimunDiameterOfBasilarArtery()));
	connect(ui->pushButtonMinimumDiameterOfLeftVetebralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToMinimumDiameterOfLeftVertebralArtery()));
	connect(ui->pushButtonMinimumDiameterOfRightVetebralArtery, SIGNAL(clicked()),
		this, SLOT(setCurrentModeToMinimumDiameterOfRightVertebralArtery()));
}
