#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
// #include "SocketActor.h"
#include "GCPlanGameInstance.generated.h"

UCLASS()
class GCPLAN_API UGCPlanGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		// ASocketActor* SocketActor;
		// TMap<FString, bool> Initeds;

		// void InitActor(FString);
		// bool IsIniteds(TArray<FString>);
};
