#pragma once

#include <SaveLoad/SaveLoadTypeIds.h>
#include <AzCore/Component/ComponentBus.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <SaveData/SaveDataRequestBus.h>

namespace SaveLoad
{
    class SaveLoadComponentRequests : public AZ::ComponentBus
    {
    public:
        ~SaveLoadComponentRequests() override = default;

        virtual void SaveBufferToPersistentStorage() = 0;
        virtual void LoadBufferFromPersistentStorage() = 0;
        virtual void SaveObjectToPersistentStorage() = 0;
        virtual void LoadObjectFromPersistentStorage(const AzFramework::LocalUserId&) = 0;
        virtual bool GetInEditor() const = 0;
        virtual void SetInEditor(const bool&) = 0;
    };

    using SaveLoadComponentRequestBus = AZ::EBus<SaveLoadComponentRequests>;

    class SaveLoadNotifications
        : public AZ::ComponentBus
    {
    public:
        virtual void OnSavedBuffer() = 0;
        virtual void OnLoadedBuffer() = 0;
        virtual void OnSavedObject() = 0;
        virtual void OnLoadedObject() = 0;
    };

    using SaveLoadNotificationBus = AZ::EBus<SaveLoadNotifications>;

    class SaveLoadNotificationHandler
        : public SaveLoadNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(SaveLoadNotificationHandler,
            SaveLoadNotificationHandlerTypeId,
            AZ::SystemAllocator, OnSavedBuffer, OnLoadedBuffer, OnSavedObject, OnLoadedObject);

        void OnSavedBuffer() override
        {
            Call(FN_OnSavedBuffer);
        }
        void OnLoadedBuffer() override
        {
            Call(FN_OnLoadedBuffer);
        }
        void OnSavedObject() override
        {
            Call(FN_OnSavedObject);
        }
        void OnLoadedObject() override
        {
            Call(FN_OnLoadedObject);
        }
    };
} // namespace SaveLoad
