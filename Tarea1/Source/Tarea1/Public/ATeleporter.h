// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATeleporter.generated.h"

class UBoxComponent;

UCLASS()
class TAREA1_API AATeleporter : public AActor
{
	GENERATED_BODY()

public:
	AATeleporter();

	//CONTROLA SI SE MODIFICA LA PROPIEDAD EN EL EDITOR
	#if WITH_EDITOR
		virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	#endif
	
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Teleporter")
	UBoxComponent* TeleportArea;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	AATeleporter* DestinationTeleporter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleporter")
	TSubclassOf<AActor> ActorTypeToTeleport;

	//EVENTOS DE CONTROL DE COLISION
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	void SetDestinationTeleporter(AATeleporter* NewDestination);
	bool bIsTeleporting;
};

