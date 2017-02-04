/*
Author:		Wong, Matthew Lun
Date:		16th, June 2016
Occupation:	Chinese University of Hong Kong,
Department of Imaging and Inteventional Radiology,
Junior Research Assistant


The abstract interactor class used in medical viewers.
This class is the ultimate parent of all interactor classes.


Wong Matthew Lun
Copyright (C) 2016
*/

#include "AbstractInteractorStyle.h"

std::list<AbstractInteractorStyle*> AbstractInteractorStyle::m_abstractInteractorStyles;

void AbstractInteractorStyle::SetCustomEnabled(bool flag)
{
	m_customFlag = flag;
}

void AbstractInteractorStyle::CustomEnabledOn()
{
	SetCustomEnabled(true);
}

void AbstractInteractorStyle::CustomEnabledOff()
{
	SetCustomEnabled(false);
}

AbstractInteractorStyle::AbstractInteractorStyle() {
	m_abstractInteractorStyles.push_back(this);
}

AbstractInteractorStyle::~AbstractInteractorStyle() {
	m_abstractInteractorStyles.remove(this);
}

void AbstractInteractorStyle::OnLeftButtonDown()
{
	m_leftFunctioning = true;
}

void AbstractInteractorStyle::OnRightButtonDown()
{
	m_rightFunctioning = true;
}

void AbstractInteractorStyle::OnMiddleButtonDown()
{
	m_middleFunctioning = true;
}

void AbstractInteractorStyle::OnLeftButtonUp()
{
	m_leftFunctioning = false;
}

void AbstractInteractorStyle::OnLeftDoubleClick()
{
}

void AbstractInteractorStyle::OnRightButtonUp()
{
	m_rightFunctioning = false;
}

void AbstractInteractorStyle::OnRightDoubleClick()
{
}

void AbstractInteractorStyle::OnMiddleButtonUp()
{
	m_middleFunctioning = false;
}

void AbstractInteractorStyle::OnMiddleDoubleClick()
{
}