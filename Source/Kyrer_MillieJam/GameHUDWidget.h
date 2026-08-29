// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "GameHUDWidget.generated.h"

UCLASS()
class KYRER_MILLIEJAM_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StabilityBar; // Имя переменной должно строго совпадать с именем в иерархии UI!

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* SatietyBar; // Имя переменной должно строго совпадать с именем в иерархии UI!


	UPROPERTY(meta = (BindWidget))
	UWidget* Panel_Pause;

	UPROPERTY(meta = (BindWidget))
	UWidget* Panel_Death;

	UPROPERTY(meta = (BindWidget))
	UWidget* Panel_Win;

	UFUNCTION(BlueprintCallable)
	void ShowPauseMenu(bool bShow) { if (IsValid(Panel_Pause)) Panel_Pause->SetVisibility(bShow ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); }

	UFUNCTION(BlueprintCallable)
	void ShowDeathMenu() { if (IsValid(Panel_Death)) Panel_Death->SetVisibility(ESlateVisibility::Visible); }

	UFUNCTION(BlueprintCallable)
	void ShowWinMenu() { if (IsValid(Panel_Win)) Panel_Win->SetVisibility(ESlateVisibility::Visible); }

	UFUNCTION(BlueprintCallable)
	void HideAllPanels()
	{
		if (Panel_Pause) Panel_Pause->SetVisibility(ESlateVisibility::Collapsed);
		if (Panel_Death) Panel_Death->SetVisibility(ESlateVisibility::Collapsed);
		if (Panel_Win) Panel_Win->SetVisibility(ESlateVisibility::Collapsed);
	}

	UFUNCTION(BlueprintCallable)
	void HandleSatietyDepleted()
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->SetInputMode(FInputModeUIOnly());
			PC->bShowMouseCursor = true;
		}
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateStability(float NewStabilityPercent); // В Unreal для процентов UI всегда используется float (от 0.0 до 1.0)

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void UpdateSatiety(float NewStabilityPercent); // В Unreal для процентов UI всегда используется float (от 0.0 до 1.0)

};
