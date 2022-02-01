// Copyright 2022 Yacob N. S. Reyneke All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Motion.generated.h"

UCLASS()
class MOTIONCOMPARATOR_API AMotion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMotion();

	UFUNCTION(BlueprintImplementableEvent, Category = "Motion")
	void OnMotionComplete();

	UFUNCTION(BlueprintImplementableEvent, Category = "Motion")
	void OnMotionFailed();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Reference to motion comparator logic component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Motion")
	class UMotionComparatorComponent* MotionComparatorComponent;

	// Must be called along with parameters for comparison to be performed
	// ReferenceComponent: Component with collision volume whose movement will be compared to this motion
	// Motions: Array of ChildActorComponent of Motion_BP that represent the entire motion
	UFUNCTION(BlueprintCallable, Category = "Motion")
	void Initialize(class UPrimitiveComponent* ReferenceComponent, TArray<class UChildActorComponent*> Motions);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
