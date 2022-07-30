// Shoot Them Up Game, All Rights Reserved


#include "Animations/STUEquipFinishedAnimNotify.h"


void USTUEquipFinishedAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnEquipAnimFinished.Broadcast(MeshComp);

    Super::Notify(MeshComp, Animation);
}
