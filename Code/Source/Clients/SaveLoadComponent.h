#pragma once
#include <SaveLoad/SaveLoadComponentBus.h>

#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <SaveLoad/SaveLoadTypeIds.h>

namespace SaveLoad
{
    class SaveLoadComponent
        : public AZ::Component
        , public SaveLoadComponentRequestBus::Handler
    {
    public:
        // AZ_RTTI(SaveLoadComponent, SaveLoadComponentTypeId);
        AZ_COMPONENT(SaveLoadComponent, SaveLoadComponentTypeId);

        // Provide runtime reflection
        static void Reflect(AZ::SerializeContext& sc);
        static void Reflect(AZ::ReflectContext* rc);

        // AZ::Component overrides
        void Activate() override;
        void Deactivate() override;

        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);

        // SaveLoadComponentRequestBus
        void SaveStringToPersistentStorage(const AZStd::string& stringSaveFilename, const AZStd::string& stringToSave) override;
        void LoadStringFromPersistentStorage(const AZStd::string& bufferLoadFilename) override;
        void SaveThisSaveLoadComponentToPersistentStorage(const AZStd::string& thisSaveLoadComponentSaveFilename) override;
        void LoadThisSaveLoadComponentFromPersistentStorage(
            const AZStd::string& thisSaveLoadComponentLoadFilename, const AzFramework::LocalUserId& localUserId) override;
        void SaveTransformComponentToPersistentStorage(
            const AZStd::string& transformComponentSaveFilename, const AZ::EntityId& entityIdToSaveTransform) override;
        void LoadTransformComponentFromPersistentStorage(
            const AZStd::string& transformComponentLoadFilename,
            const AZ::EntityId& entityIdToLoadTransform,
            const AzFramework::LocalUserId& localUserId) override;
        AZStd::string GetLastStringSaveLoadFilename() const override;
        AZStd::string GetLastThisSaveLoadComponentSaveLoadFilename() const override;
        AZStd::string GetLastTransformComponentSaveLoadFilename() const override;
        bool GetInEditor() const override;
        void SetInEditor(const bool& new_inEditor) override;
        bool GetTestBool() const override;
        void SetTestBool(const bool& new_testBool) override;

    private:
        // SaveLoadNotificationBus
        void OnSavedStringFile();
        void OnLoadedStringFile(const AZStd::string& loadedStringFilename, const AZStd::string& loadedString);
        void OnSavedThisSaveLoadComponentFile();
        void OnLoadedThisSaveLoadComponentFile();
        void OnSavedTransformComponentFile();
        void OnLoadedTransformComponentFile();

        AZStd::string m_stringSaveLoadFilename = "StringSaveData";
        AZStd::string m_thisSaveLoadComponentSaveLoadFilename = "SaveLoadComponentSaveData";
        AZStd::string m_transformComponentSaveLoadFilename = "TransformComponentSaveData";
        bool m_inEditor = false;

        AZStd::string m_loadedString;

        bool m_testBool = false;
    };
} // namespace SaveLoad
