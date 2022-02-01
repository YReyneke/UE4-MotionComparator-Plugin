// Copyright 2022 Yacob N. S. Reyneke All Rights Reserved.


#include "MotionPoint.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h" 	

// Sets default values
AMotionPoint::AMotionPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PointCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PointCollisionBox"));
	SetRootComponent(PointCollisionBox);

	MovementCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MovementCollisionBox"));
	MovementCollisionBox->SetupAttachment(PointCollisionBox);

	Indicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MotionArrow"));
	Indicator->SetupAttachment(PointCollisionBox);
}

// Called when the game starts or when spawned
void AMotionPoint::BeginPlay()
{
	Super::BeginPlay();
	
	baseIndicatorScale = Indicator->GetComponentScale();

	if (!bHasMotion) {
		MovementCollisionBox->DestroyComponent();
	}
}

// Called every frame
void AMotionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Check if ReferenceComponent is within bounds of this motion point
PointStatus AMotionPoint::VerifyPosition() {
	if (bInitialized) { // If setup function has been called
		// Set motion flag to failed if not overlapping reference component when it should be
		if (bHasMotion && !bLastPoint) {
			if (!MovementCollisionBox->IsOverlappingComponent(ReferenceComponent)) {
				UE_LOG(LogTemp, Warning, TEXT("MotionPoint.VerifyPosition: Motion point collision failed\n"
					"Component: %s\nPointPos: %s\nComp Pos: %s"),
					*ReferenceComponent->GetName(), *GetActorLocation().ToString(), *ReferenceComponent->GetComponentLocation().ToString());
				MotionStatus = Failed;
			}
		}
		else {
			if (!PointCollisionBox->IsOverlappingComponent(ReferenceComponent)) {
				UE_LOG(LogTemp, Warning, TEXT("MotionPoint.VerifyPosition: Static point collision failed\n"
					"Component: %s\nPointPos: %s\nComp Pos: %s"),
					*ReferenceComponent->GetName(), *GetActorLocation().ToString(), *ReferenceComponent->GetComponentLocation().ToString());
				MotionStatus = Failed;
			}
		}
	}
	return MotionStatus;
}

// NOTE: NexPointRotation is ignored if bLastPoint
// NOTE: If tolerance axis value <= 0, that axis is ignored
PointStatus AMotionPoint::VerifyRotation(FVector RotationTolerance, FVector NextPointRelativeRotation) {
	if (bInitialized) { // If setup function has been called
		bool bIgnoreX = (RotationTolerance.X <= 0) ? true : false;
		bool bIgnoreY = (RotationTolerance.Y <= 0) ? true : false;
		bool bIgnoreZ = (RotationTolerance.Z <= 0) ? true : false;

		FVector ReferenceRotation = ReferenceComponent->GetComponentRotation().Euler();
		ReferenceRotation -= GetActorRotation().Euler(); // Reference rotation now contains the delta between this actor and reference component rotations
		if (bLastPoint) {
			if (!bIgnoreX)
				MotionStatus = (FGenericPlatformMath::Abs(ReferenceRotation.X) <= RotationTolerance.X) ? MotionStatus : Failed;
			if (!bIgnoreY)
				MotionStatus = (FGenericPlatformMath::Abs(ReferenceRotation.Y) <= RotationTolerance.Y) ? MotionStatus : Failed;
			if (!bIgnoreZ)
				MotionStatus = (FGenericPlatformMath::Abs(ReferenceRotation.Z) <= RotationTolerance.Z) ? MotionStatus : Failed;
		}
		else {
			if (!bIgnoreX) {
				if (NextPointRelativeRotation.X < 0) {
					if (ReferenceRotation.X > RotationTolerance.X ||
						ReferenceRotation.X < NextPointRelativeRotation.X - RotationTolerance.X)
						MotionStatus = Failed;
				}
				else {
					if (ReferenceRotation.X < -1 * RotationTolerance.X ||
						ReferenceRotation.X > NextPointRelativeRotation.X + RotationTolerance.X)
						MotionStatus = Failed;
				}
			}
			if (!bIgnoreY) {
				if (NextPointRelativeRotation.Y < 0) {
					if (ReferenceRotation.Y > RotationTolerance.Y ||
						ReferenceRotation.Y < NextPointRelativeRotation.Y - RotationTolerance.Y)
						MotionStatus = Failed;
				}
				else {
					if (ReferenceRotation.Y < -1 * RotationTolerance.Y ||
						ReferenceRotation.Y > NextPointRelativeRotation.Y + RotationTolerance.Y)
						MotionStatus = Failed;
				}
			}
			if (!bIgnoreZ) {
				if (NextPointRelativeRotation.Z < 0) {
					if (ReferenceRotation.Z > RotationTolerance.Z ||
						ReferenceRotation.Z < NextPointRelativeRotation.Z - RotationTolerance.Z)
						MotionStatus = Failed;
				}
				else {
					if (ReferenceRotation.Z < -1 * RotationTolerance.Z ||
						ReferenceRotation.Z > NextPointRelativeRotation.Z + RotationTolerance.Z)
						MotionStatus = Failed;
				}
			}
		}
	}
	if (MotionStatus == Failed) {
		UE_LOG(LogTemp, Warning, TEXT("MotionPoint.VerifyRotation: Motion point rotation failed\n"
			"Component: %s\nPointPos: %s\nComp Pos: %s"),
			*ReferenceComponent->GetName(), *GetActorRotation().ToString(), *ReferenceComponent->GetComponentRotation().ToString());
	}
	return MotionStatus;
}

bool AMotionPoint::VerifyAsEndPoint(FVector RotationTolerance) {
	bool retVal = true;
	if (bInitialized) {
		if (!PointCollisionBox->IsOverlappingComponent(ReferenceComponent)) {
			return false;
		}

		bool bIgnoreX = (RotationTolerance.X <= 0) ? true : false;
		bool bIgnoreY = (RotationTolerance.Y <= 0) ? true : false;
		bool bIgnoreZ = (RotationTolerance.Z <= 0) ? true : false;

		FVector ReferenceRotation = ReferenceComponent->GetComponentRotation().Euler();
		ReferenceRotation -= GetActorRotation().Euler(); // Reference rotation now contains the delta between this actor and reference component rotations

		if (!bIgnoreX)
			retVal = (FGenericPlatformMath::Abs(ReferenceRotation.X) <= RotationTolerance.X) ? retVal : false;
		if (!bIgnoreY)
			retVal = (FGenericPlatformMath::Abs(ReferenceRotation.Y) <= RotationTolerance.Y) ? retVal : false;
		if (!bIgnoreZ)
			retVal = (FGenericPlatformMath::Abs(ReferenceRotation.Z) <= RotationTolerance.Z) ? retVal : false;
	}
	return retVal;
}

void AMotionPoint::Initialize(const int Index, UPrimitiveComponent* MotionReferenceComponent)
{
	PointIndex = Index;
	ReferenceComponent = MotionReferenceComponent;
	if (PointIndex == 0) { // Set first motion to 'started'
		MotionStatus = Started;
	}
	bInitialized = true;
}

void AMotionPoint::UdpateIndicatorMaterial(UMaterial* newMat) {
	Indicator->SetMaterial(0, newMat);
}

// If defaultScale is set to true, newScale is ignored
void AMotionPoint::SetIndicatorScale(bool defaultScale, float newScale) {
	if (defaultScale) {
		Indicator->SetWorldScale3D(baseIndicatorScale);
	}
	else {
		Indicator->SetWorldScale3D(baseIndicatorScale * newScale);
	}
}

void AMotionPoint::SetIndicatorVisible(bool Visible)
{
	Indicator->SetVisibility(Visible);
}

void AMotionPoint::SetEndPoint() {
	bLastPoint = true;
	MovementCollisionBox->DestroyComponent();
}