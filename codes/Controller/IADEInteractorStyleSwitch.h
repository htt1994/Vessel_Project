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


#ifndef __IADE_INTERACTORSTYLE_SWITCH_H__
#define __IADE_INTERACTORSTYLE_SWITCH_H__

#include "InteractorStyleSwitch.h"

#include "QInteractorStyleNavigation.h"
#include "InteractorStyleWindowLevel.h"


class InteractorStyleWindowLevel;
class QInteractorStyleNavigation;

class IADEInteractorStyleSwitch : public InteractorStyleSwitch
{
public:
	static IADEInteractorStyleSwitch* New();
	vtkTypeMacro(IADEInteractorStyleSwitch, InteractorStyleSwitch);


	vtkGetMacro(WindowLevel, InteractorStyleWindowLevel*);
	SetInteractorStyleMacro(WindowLevel);
	CurrentStyleMacro(WindowLevel);

	vtkGetMacro(Navigation, QInteractorStyleNavigation*);
	SetInteractorStyleMacro(Navigation);
	CurrentStyleMacro(Navigation);
	//vtkGetMacro(PolygonDraw, QInteractorStyleVesselSegmentation*);
	//vtkGetMacro(SeedsPlacer, QInteractorStyleLumenSeedsPlacer*);
	//vtkGetMacro(PaintBrush, QInteractorStylePaintBrush*);
	//vtkGetMacro(ROI, QInteractorStyleROI*);
	//vtkGetMacro(Ruler, QInteractorStyleRuler*);


	//SetInteractorStyleMacro(InteractorStyleTesting);
	//SetInteractorStyleMacro(Navigation);
	//void SetInteractorStyleToNavigation();
	//SetInteractorStyleMacro(WindowLevel);
	//SetInteractorStyleMacro(PolygonDraw);
	//SetInteractorStyleMacro(SeedsPlacer);
	//SetInteractorStyleMacro(PaintBrush);
	//SetInteractorStyleMacro(ROI);
	//SetInteractorStyleMacro(Ruler);

	//CurrentStyleMacro(InteractorStyleTesting);
	//CurrentStyleMacro(Navigation);
	//CurrentStyleMacro(WindowLevel);
	//CurrentStyleMacro(PolygonDraw);
	//CurrentStyleMacro(SeedsPlacer);
	//CurrentStyleMacro(PaintBrush);
	//CurrentStyleMacro(ROI);
	//CurrentStyleMacro(Ruler);

protected:
	IADEInteractorStyleSwitch();

	QInteractorStyleNavigation*	Navigation;
	InteractorStyleWindowLevel* WindowLevel;
	//QInteractorStyleVesselSegmentation* PolygonDraw;
	//QInteractorStyleLumenSeedsPlacer* SeedsPlacer;
	//QInteractorStylePaintBrush*	PaintBrush;
	//QInteractorStyleROI* ROI;
	//QInteractorStyleRuler* Ruler;
};



#endif