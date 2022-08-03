#pragma once

class AnimUtils
{
public:    
    template <typename T>
    static T* FindFirstNotifyByClass(UAnimSequenceBase* Anim)
    {
        if (!Anim)
            return nullptr;

        const auto AnimNotifyEvents = Anim->Notifies;

        for (auto& AnimNotifyEvent : AnimNotifyEvents)
        {
            auto AnimNotify = Cast<T>(AnimNotifyEvent.Notify);

            if (AnimNotify)
            {
                return AnimNotify;
            }
        }
        return nullptr;
    }
};