// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUDWidget.h"
#include "Components/ProgressBar.h"

void UGameHUDWidget::UpdateStability(float NewHealthPercent)
{
    // Проверяем, существует ли полоска здоровья, чтобы игра не крашнулась
    if (StabilityBar)
    {
        // У ProgressBar в Unreal есть встроенный метод SetPercent
        StabilityBar->SetPercent(NewHealthPercent);
    }
}
void UGameHUDWidget::UpdateSatiety(float NewHealthPercent)
{
    // Проверяем, существует ли полоска здоровья, чтобы игра не крашнулась
    if (SatietyBar)
    {
        // У ProgressBar в Unreal есть встроенный метод SetPercent
        SatietyBar->SetPercent(NewHealthPercent);
    }
}