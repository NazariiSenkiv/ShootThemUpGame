// Shoot Them Up Game, All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include <type_traits>

#include "STUPlayerHUDWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUPlayerHUDWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UFUNCTION(BlueprintCallable, Category = "UI")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    FLinearColor GetHealthBarColor() const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

private:
    template <typename TComponentClass>
    TComponentClass* GetOwningPlayerComponent() const
    {
        static_assert(std::is_base_of_v<UActorComponent, TComponentClass>, "TComponentClass is not derived from UActorComponent");

        const auto PlayerPawn = GetOwningPlayerPawn();
        if (!PlayerPawn)
            return nullptr;

        const auto Component = PlayerPawn->GetComponentByClass(TComponentClass::StaticClass());
        const auto WeaponComponent = Cast<TComponentClass>(Component);

        return WeaponComponent;
    }
};
