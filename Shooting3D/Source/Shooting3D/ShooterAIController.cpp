// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"
void AShooterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIBehavior != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("in"));
		RunBehaviorTree(AIBehavior);
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
		GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());
	}
	
	//

	//SetFocus(PlayerPawn);
}

bool AShooterAIController::IsDead()
{
	AShooterCharacter* Character = Cast<AShooterCharacter>(GetPawn());
	if (Character != nullptr)
	{
		return Character->IsDead();
	}
	return true;
}

void AShooterAIController::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	//APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	//if (LineOfSightTo(PlayerPawn))
	//{
	//	GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation());
	//	GetBlackboardComponent()->SetValueAsVector(TEXT("LastLocation"), PlayerPawn->GetActorLocation());
	//	//MoveToActor(PlayerPawn, AcceptanceRadius);
	//	//SetFocus(PlayerPawn);
	//}
	//else {
	//	GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
	//	//StopMovement();
	//}
}