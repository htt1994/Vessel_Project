#include "QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter.h"
#include "ui_QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter);
QSETUP_UI_SRC(QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter);

void QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter::FindMaximumRadius()
{
	InteractorStyleSurfaceCenterLineDistanceFindMaximumRadius::FindMaximumRadius();
	ui->doubleSpinBoxMinimumDiameterOfBasilarArtery->setValue(m_minRadius * 2);
}

QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter::QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter::~QInteractorStyleSurfaceCenterLineVBDSmokerBADiameter()
{
	QDELETE_UI();
}

