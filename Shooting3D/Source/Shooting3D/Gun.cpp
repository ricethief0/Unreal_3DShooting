// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SetRootComponent(Root);
	Mesh->SetupAttachment(Root);

	Ammo = AmmoMax;
}

void AGun::PullTrigger()
{

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr)return;
	
	if(OwnerPawn->IsPlayerControlled())
	{
		if (Ammo <= 0) 
		{
			UGameplayStatics::SpawnSoundAttached(EmptyAmmoSound, Mesh, TEXT("MuzzleFlashSocket"));
			return;
		}
		Ammo--;
	}

	
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlesh, Mesh, TEXT("MuzzleFlashSocket"));
	
	

	
	FVector Lotation;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Lotation, Rotation);

	FVector End = Lotation + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Lotation, End, ECollisionChannel::ECC_GameTraceChannel1,Params);

	if (bSuccess)
	{
		//DrawDebugPoint(GetWorld(), Hit.Location, 20.0f, FColor::Red, true);
		FVector ShotDirection = -Rotation.Vector();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactHit, Hit.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Hit.Location);
		//UGameplayStatics::SpawnSoundAtLocation(GetWorld(),MuzzleSound,Hit.Lo)
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
	//DrawDebugCamera(GetWorld(), Lotation, Rotation, 90, 2.0f, FColor::Red, true);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

