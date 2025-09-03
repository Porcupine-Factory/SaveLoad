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

        virtual void SaveStringToPersistentStorage(const AZStd::string&, const AZStd::string&) = 0;
        virtual void LoadStringFromPersistentStorage(const AZStd::string&) = 0;
        virtual void SaveThisSaveLoadComponentToPersistentStorage(const AZStd::string&) = 0;
        virtual void LoadThisSaveLoadComponentFromPersistentStorage(const AZStd::string&, const AzFramework::LocalUserId&) = 0;
        virtual void SaveTransformComponentToPersistentStorage(const AZStd::string&, const AZ::EntityId&) = 0;
        virtual void LoadTransformComponentFromPersistentStorage(const AZStd::string&, const AZ::EntityId&, const AzFramework::LocalUserId&) = 0;
        virtual AZStd::string GetLastStringSaveLoadFilename() const = 0;
        virtual AZStd::string GetLastThisSaveLoadComponentSaveLoadFilename() const = 0;
        virtual AZStd::string GetLastTransformComponentSaveLoadFilename() const = 0;
        virtual bool GetInEditor() const = 0;
        virtual void SetInEditor(const bool&) = 0;
        virtual bool GetTestBool() const = 0;
        virtual void SetTestBool(const bool&) = 0;
    };

    using SaveLoadComponentRequestBus = AZ::EBus<SaveLoadComponentRequests>;

    class SaveLoadNotifications
        : public AZ::ComponentBus
    {
    public:
        virtual void OnSavedStringFile() = 0;
        virtual void OnLoadedStringFile(const AZStd::string&) = 0;
        virtual void OnSavedThisSaveLoadComponentFile() = 0;
        virtual void OnLoadedThisSaveLoadComponentFile() = 0;
        virtual void OnSavedTransformComponentFile() = 0;
        virtual void OnLoadedTransformComponentFile() = 0;
    };

    using SaveLoadNotificationBus = AZ::EBus<SaveLoadNotifications>;

    class SaveLoadNotificationHandler
        : public SaveLoadNotificationBus::Handler
        , public AZ::BehaviorEBusHandler
    {
    public:
        AZ_EBUS_BEHAVIOR_BINDER(SaveLoadNotificationHandler,
            SaveLoadNotificationHandlerTypeId,
            AZ::SystemAllocator, OnSavedStringFile, OnLoadedStringFile, OnSavedThisSaveLoadComponentFile, OnLoadedThisSaveLoadComponentFile, OnSavedTransformComponentFile, OnLoadedTransformComponentFile);

        void OnSavedStringFile() override
        {
            Call(FN_OnSavedStringFile);
        }
        void OnLoadedStringFile(const AZStd::string& loadedString) override
        {
            Call(FN_OnLoadedStringFile, loadedString);
        }
        void OnSavedThisSaveLoadComponentFile() override
        {
            Call(FN_OnSavedThisSaveLoadComponentFile);
        }
        void OnLoadedThisSaveLoadComponentFile() override
        {
            Call(FN_OnLoadedThisSaveLoadComponentFile);
        }
        void OnSavedTransformComponentFile() override
        {
            Call(FN_OnSavedTransformComponentFile);
        }
        void OnLoadedTransformComponentFile() override
        {
            Call(FN_OnLoadedTransformComponentFile);
        }
    };
} // namespace SaveLoad
