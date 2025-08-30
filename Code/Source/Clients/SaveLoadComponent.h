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
        void SaveBufferToPersistentStorage() override;
        void LoadBufferFromPersistentStorage() override;
        void SaveObjectToPersistentStorage() override;
        void LoadObjectFromPersistentStorage(const AzFramework::LocalUserId& localUserId) override;
        bool InEditor() const override;

    private:
        // SaveLoadNotificationBus
        void OnSavedBuffer();
        void OnLoadedBuffer();
        void OnSavedObject();
        void OnLoadedObject();

        AZStd::string testString;
        float testFloat = 0.0f;
        int testInt = 0;
        bool testBool = false;

        static const AZ::u64 testSaveDataSize = 9;
        const char* testSaveDataName = "TestSaveData";
        char testSaveData[testSaveDataSize] = {'a', 'b', 'c', '1', '2', '3', 'x', 'y', 'z'};
    };
} // namespace SaveLoad
