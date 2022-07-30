// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUEquipFinishedAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipAnimFinishedSignature, USkeletalMeshComponent*);

UCLASS()
class SHOOTTHEMUP_API USTUEquipFinishedAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

protected:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
    FOnEquipAnimFinishedSignature OnEquipAnimFinished;
};
