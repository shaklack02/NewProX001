// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Components/BoxComponent.h"

#include "MyActor_Test.generated.h"


UCLASS()
class MYPROJECT_API AMyActor_Test : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AMyActor_Test();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Pick_Up)
		float speed;

	//Rotation
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Pick_Up)
	FRotator Rotation_Rate; 

	//SceneComponent;
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Pick_Up)
	USceneComponent* SceneComponent; 
    
	// Adding A Mesh To the Actor MeshhComponent		 						
	UPROPERTY(EditAnyWhere, BluePrintReadWrite, Category = Pick_Up)
    UStaticMeshComponent* SampleMesh; 
	
	//Box Collider and overlap Function
	UPROPERTY(EditAnyWhere)
	     UBoxComponent* BoxCollider;
	UPROPERTY(EditAnyWhere)
		FString Name;

	virtual void OnInteract();
		  


	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		
	
	
};
