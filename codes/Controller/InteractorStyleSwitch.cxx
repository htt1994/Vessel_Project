/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant
This class is based on vtkInteractorStyleSwitch, written to allow easy
switching between 2D interactors.
Wong Matthew Lun
Copyright (C) 2016
*/
#include "InteractorStyleSwitch.h"

#include "InteractorStyleNavigation.h"
#include "InteractorStyleWindowLevel.h"

#include <vtkCommand.h>
#include <vtkCallbackCommand.h>	
#include <vtkObjectFactory.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

vtkStandardNewMacro(InteractorStyleSwitch);

SET_CURRENT_STYLE_TO_MACRO_CPP(Navigation);
SET_CURRENT_STYLE_TO_MACRO_CPP(WindowLevel);

InteractorStyleSwitch::InteractorStyleSwitch()
{
	InteractorStyleTesting = vtkInteractorStyleImage::New();
	// The first instance of QInteractorStyleNavigation should have control 
	// of the ui
	Navigation = InteractorStyleNavigation::New();
	WindowLevel = InteractorStyleWindowLevel::New();
	//PolygonDraw = QInteractorStyleVesselSegmentation::New();
	//SeedsPlacer = QInteractorStyleLumenSeedsPlacer::New();
	//PaintBrush = QInteractorStylePaintBrush::New();
	//ROI = QInteractorStyleROI::New();
	//Ruler = QInteractorStyleRuler::New();

	allStyles.push_back(InteractorStyleTesting);
	allStyles.push_back(WindowLevel);
	allStyles.push_back(Navigation);
	//allStyles.push_back(PolygonDraw);
	//allStyles.push_back(SeedsPlacer);
	//allStyles.push_back(PaintBrush);
	//allStyles.push_back(ROI);
	//allStyles.push_back(Ruler);

	this->CurrentStyle = 0;
}

InteractorStyleSwitch::~InteractorStyleSwitch()
{
	/* Do not modifiy the lower code�� */
	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		if ((*it) != NULL) {
			(*it)->Delete();
			(*it) = NULL;
		}
	}
	allStyles.clear();

}

void InteractorStyleSwitch::InternalUpdate()
{

	// using enable update to call #EnabledOn() and #EnabledOff()
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {

		AbstractInteractorStyle* _style = dynamic_cast<AbstractInteractorStyle*>(*cit);
		if (_style != NULL) {
			if (this->CurrentStyle == *cit) {
				_style->CustomEnabledOn();
			}
			else {
				_style->CustomEnabledOff();
			}
		}

	}
	//if (this->CurrentStyle != this->Navigation)
	//	this->Navigation->SetNavigationModeEnabled(false);
	//if (this->CurrentStyle != this->WindowLevel)
	//	this->WindowLevel->SetWindowLevelModeEnabled(false);
	//if (this->CurrentStyle != this->PolygonDraw) 
	//	this->PolygonDraw->SetPolygonModeEnabled(false);
	//if (this->CurrentStyle != this->SeedsPlacer)
	//	this->SeedsPlacer->SetSeedsPlacerEnable(false);

	//if (this->CurrentStyle != this->PaintBrush)
	//	this->PaintBrush->SetPaintBrushModeEnabled(false);
	//if (this->CurrentStyle != this->Ruler)
	//	this->Ruler->SetDistanceWidgetEnabled(false);
	//if (this->CurrentStyle != this->ROI)
	//	this->ROI->SetROIWidgetEnabled(false);

	//// some special cases need to use InternalUpdate() to enabled
	//if (this->CurrentStyle == this->Navigation)
	//	this->Navigation->SetNavigationModeEnabled(true);
	//if (this->CurrentStyle == this->WindowLevel)
	//	this->WindowLevel->SetWindowLevelModeEnabled(true);
	//if (this->CurrentStyle == this->PolygonDraw)
	//	this->PolygonDraw->SetPolygonModeEnabled(true);
	//if (this->CurrentStyle == this->SeedsPlacer)
	//	this->SeedsPlacer->SetSeedsPlacerEnable(true);

	//if (this->CurrentStyle == this->ROI)
	//	this->ROI->SetROIWidgetEnabled(true);
	//if (this->CurrentStyle == this->PaintBrush)
	//	this->PaintBrush->SetPaintBrushModeEnabled(true);
	//if (this->CurrentStyle == this->Ruler)
	//	this->Ruler->SetDistanceWidgetEnabled(true);
}

void InteractorStyleSwitch::SetAutoAdjustCameraClippingRange(int value)
{
	if (value == this->AutoAdjustCameraClippingRange)
	{
		return;
	}

	if (value < 0 || value > 1)
	{
		vtkErrorMacro("Value must be between 0 and 1 for" <<
			" SetAutoAdjustCameraClippingRange");
		return;
	}

	for (std::list<vtkInteractorStyle*>::iterator it = allStyles.begin();
		it != allStyles.end(); ++it) {
		(*it)->SetAutoAdjustCameraClippingRange(value);
	}
	this->Modified();
}

void InteractorStyleSwitch::SetDefaultRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetDefaultRenderer(renderer);

	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetDefaultRenderer(renderer);
	}
}

void InteractorStyleSwitch::SetCurrentRenderer(vtkRenderer* renderer)
{
	vtkInteractorStyle::SetCurrentRenderer(renderer);

	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetCurrentRenderer(renderer);
	}
}

void InteractorStyleSwitch::SetImageViewer(ImageViewer* imageViewer)
{
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		AbstractNavigation* _style = 
			AbstractNavigation::SafeDownCast(*cit);
		if (_style != NULL) {
			_style->SetImageViewer(imageViewer);
		}
	}
}

void InteractorStyleSwitch::SetEnabled(int i)
{
	Superclass::SetEnabled(i);
	for (std::list<vtkInteractorStyle*>::const_iterator cit = allStyles.cbegin();
		cit != allStyles.cend(); ++cit) {
		(*cit)->SetEnabled(i);
	}
}

void InteractorStyleSwitch::SetEnabledOn()
{
	this->SetEnabled(1);
}

void InteractorStyleSwitch::SetEnabledOff()
{
	this->SetEnabled(0);
}


void InteractorStyleSwitch::PrintSelf(ostream & os, vtkIndent indent)
{
	vtkInteractorStyleSwitchBase::PrintSelf(os, indent);
}

void InteractorStyleSwitch::SetInteractor(vtkRenderWindowInteractor *iren)
{
	if (iren == this->Interactor)
	{
		return;
	}
	// if we already have an Interactor then stop observing it
	if (this->Interactor)
	{
		this->Interactor->RemoveObserver(this->EventCallbackCommand);
	}
	this->Interactor = iren;
	// add observers for each of the events handled in ProcessEvents
	if (iren)
	{
		iren->AddObserver(vtkCommand::CharEvent,
			this->EventCallbackCommand,
			this->Priority);

		iren->AddObserver(vtkCommand::DeleteEvent,
			this->EventCallbackCommand,
			this->Priority);
	}
}

//void InteractorStyleSwitch::SetInteractorStyleToNavigation()
//{
//
//	if (!this->CurrentStyleIsNavigation()) {
//		if (this->CurrentStyle) {
//			this->CurrentStyle->SetInteractor(0);
//		}
//		this->CurrentStyle = (vtkInteractorStyle*) this->Navigation;
//	}
//	if (this->CurrentStyle) {
//		this->CurrentStyle->SetInteractor(this->Interactor);
//		this->CurrentStyle->SetTDxStyle(this->TDxStyle);
//	}
//	this->InternalUpdate();
//}

