#pragma once
#include <SaveLoad/SaveLoadComponentBus.h>

#include <SaveLoad/SaveLoadTypeIds.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>

namespace SaveLoad
{
    class SaveLoadComponent
        : public AZ::Component
        , public SaveLoadComponentRequestBus::Handler
    {
    public:
        //AZ_RTTI(SaveLoadComponent, SaveLoadComponentTypeId);
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
        void SaveBufferToPersistentStorage(const AZStd::string& strBufferToSave) override;
        AZStd::string LoadBufferFromPersistentStorage() override;
        void SaveObjectToPersistentStorage() override;
        void LoadObjectFromPersistentStorage(const AzFramework::LocalUserId& localUserId) override;
        AZStd::string GetBufferSaveFilename() const override;
        void SetBufferSaveFilename(const AZStd::string& new_bufferSaveFilename) override;
        AZStd::string GetObjectSaveFilename() const override;
        void SetObjectSaveFilename(const AZStd::string& new_objectSaveFilename) override;
        bool GetInEditor() const override;
        void SetInEditor(const bool& new_inEditor) override;
        bool GetTestBool() const override;
        void SetTestBool(const bool& new_testBool) override;

    private:
        // SaveLoadNotificationBus
        void OnSavedBuffer();
        void OnLoadedBuffer();
        void OnSavedObject();
        void OnLoadedObject();

        bool m_inEditor = false;
        AZStd::string m_bufferSaveFilename = "BufferSaveData";
        AZStd::string m_objectSaveFilename = "ObjectSaveData";

        AZStd::string m_loadedBuffer;

        AZStd::string m_testString;
        float m_testFloat = 0.0f;
        int m_testInt = 0;
        bool m_testBool = false;
    };
} // namespace SaveLoad
