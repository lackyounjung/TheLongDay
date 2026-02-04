// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingWidget.h"

bool ULoadingWidget::CallBlueprintFunction(float percent)
{
	m_num = percent;
	setRoundPercent(percent);
	
	return true;
}
