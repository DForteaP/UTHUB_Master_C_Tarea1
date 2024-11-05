#include "Tarea1/Public/ATeleporter.h"
#include "Components/BoxComponent.h"

AATeleporter::AATeleporter()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsTeleporting = false;
	TeleportArea = CreateDefaultSubobject<UBoxComponent>(TEXT("TeleportArea"));
	RootComponent = TeleportArea;
	
	TeleportArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TeleportArea->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	TeleportArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	
	TeleportArea->OnComponentBeginOverlap.AddDynamic(this, &AATeleporter::OnOverlapBegin);
	TeleportArea->OnComponentEndOverlap.AddDynamic(this, &AATeleporter::OnOverlapEnd);
}

void AATeleporter::BeginPlay()
{
	Super::BeginPlay();
}

void AATeleporter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsTeleporting) return;
	if (DestinationTeleporter && OtherActor && OtherActor->IsA(ActorTypeToTeleport))
	{
		DestinationTeleporter->bIsTeleporting = true;
		FVector DestinationLocation = DestinationTeleporter->GetActorLocation();
		OtherActor->SetActorLocation(DestinationLocation);
	}
}

void AATeleporter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ActorTypeToTeleport))
	{
		bIsTeleporting = false;
	}
}

void AATeleporter::SetDestinationTeleporter(AATeleporter* NewDestination)
{
	DestinationTeleporter = NewDestination;
	if (NewDestination && NewDestination->DestinationTeleporter != this)
	{
		NewDestination->SetDestinationTeleporter(this);
	}
}

#if WITH_EDITOR
	void AATeleporter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);

		FName PropertyName;
		if (PropertyChangedEvent.Property)
		{
			PropertyName = PropertyChangedEvent.Property->GetFName();
		} else{
			PropertyName = NAME_None;
		}
		
		if (PropertyName == GET_MEMBER_NAME_CHECKED(AATeleporter, DestinationTeleporter))
		{
			SetDestinationTeleporter(DestinationTeleporter);
		}
	}
#endif