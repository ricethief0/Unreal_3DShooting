// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
#include "EngineUtils.h"
#include "ShooterAIController.h"
#include "GameFramework/Controller.h"
#include  "Kismet/GameplayStatics.h"

void AKillEmAllGameMode::BeginPlay()
{
	UGameplayStatics::PlaySound2D(GetWorld(), BGM);
}

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);
	
	APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
	if (PlayerController != nullptr)
	{
		//PlayerController->GameHasEnded(nullptr,false);
		EndGame(false);
	
	}

	for (AShooterAIController* AIController : TActorRange<AShooterAIController>(GetWorld()))
	{
		if (!AIController->IsDead()) return; 
	}
	
	EndGame(true);
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
	}
}


