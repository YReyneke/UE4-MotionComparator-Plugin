// Copyright 2022 Yacob N. S. Reyneke All Rights Reserved.


#include "Motion.h"
#include "MotionComparatorComponent.h"
#include "MotionPoint.h"
#include "Components/ChildActorComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
AMotion::AMotion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionComparatorComponent = CreateDefaultSubobject<UMotionComparatorComponent>(TEXT("MotionComparatorComponent"));
	SetRootComponent(MotionComparatorComponent);
}

// Called when the game starts or when spawned
void AMotion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMotion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMotion::Initialize(UPrimitiveComponent* ReferenceComponent, TArray<UChildActorComponent*> Motions) {
	if (!ReferenceComponent->IsValidLowLevel()) {
		UE_LOG(LogTemp, Error, TEXT("AMotion.Initialize: ReferenceComponent Invalid!"));
		return;
	}
	if (Motions.Num() < 2) {
		UE_LOG(LogTemp, Error, TEXT("AMotion.Initialize: Motions list does not contain enough AMotionPoint_BP references!"));
		return;
	}

	// Set world location and rotation to starting transfrom
	SetActorLocationAndRotation(ReferenceComponent->GetComponentLocation(), FRotator(0.f, ReferenceComponent->GetComponentRotation().Yaw, 0.f));

	MotionComparatorComponent->ReferenceComponent = ReferenceComponent;

	for (int i{ 0 }; i < Motions.Num(); i++) {
		UChildActorComponent* point = Motions[i];
		point->CreateChildActor();
		AMotionPoint* pointRef = Cast<AMotionPoint>(point->GetChildActor());
		pointRef->Initialize(i, ReferenceComponent);
		MotionComparatorComponent->MotionPoints.Add(pointRef);
	}

	MotionComparatorComponent->Start();
}