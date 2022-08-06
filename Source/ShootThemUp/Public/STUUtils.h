#pragma once

#include <type_traits>

class STUUtils
{
public:
    template<typename TComponentClass>
    static TComponentClass* GetPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn)
            return nullptr;
        
        static_assert(std::is_base_of_v<UActorComponent, TComponentClass>, "TComponentClass is not derived from UActorComponent");
        
        const auto Component = PlayerPawn->GetComponentByClass(TComponentClass::StaticClass());
        return Cast<TComponentClass>(Component); 
    }
};