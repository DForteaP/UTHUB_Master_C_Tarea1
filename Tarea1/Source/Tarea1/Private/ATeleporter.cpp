#include "Tarea1/Public/ATeleporter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

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
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		APlayerCameraManager* CameraManager = nullptr;
		
		if (PlayerController)
		{
			CameraManager = PlayerController->PlayerCameraManager;
			// DESACTIVA MOVIMIENTO MIENTRAS SE TELETRANSPORTA
			ACharacter* Character = Cast<ACharacter>(OtherActor);
			if (Character)
			{
				Character->DisableInput(PlayerController);
			}
			if (CameraManager)
			{
				CameraManager->StartCameraFade(0.0f, 1.0f, 1.0f, FLinearColor::Black, true, true);
			}
		}
		DestinationTeleporter->bIsTeleporting = true;

		//TIMER PARA CONTROLAR EL FADE
		FTimerHandle TeleportTimer;
		GetWorld()->GetTimerManager().SetTimer(TeleportTimer, [this, OtherActor, CameraManager, PlayerController]()
		{
			FVector DestinationLocation = DestinationTeleporter->GetActorLocation();
			OtherActor->SetActorLocation(DestinationLocation);
			if (CameraManager)
			{
				CameraManager->StartCameraFade(1.0f, 0.0f, 1.0f, FLinearColor::Black, true, true);
			}
			// DEVUELVE MOVIMIENTO MIENTRAS TRAS TELETRANSPORTARSE
			ACharacter* Character = Cast<ACharacter>(OtherActor);
			if (Character)
			{
				Character->EnableInput(PlayerController);
			}
		}, 1.0f, false);

	
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