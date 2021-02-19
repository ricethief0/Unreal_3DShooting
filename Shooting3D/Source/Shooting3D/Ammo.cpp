// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "Components/StaticMeshComponent.h"
//#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

// Sets default values
AAmmo::AAmmo()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnOverlapBegin);
	//Mesh->OnComponentEndOverlap.AddDynamic(this, &AAmmo::OnOverlapEnd);
	
	
}

// Called when the game starts or when spawned
void AAmmo::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.f, RotateRate * DeltaTime, 0.f));
}

void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Player = Cast<APawn>(OtherActor);
	AShooterPlayerController* Controller = Cast<AShooterPlayerController>(Player->GetController());
	if (Controller != nullptr )
	{
		Cast<AShooterCharacter>(Player)->AmmoCharge(AmmoCount);
		Destroy();
	}
}



