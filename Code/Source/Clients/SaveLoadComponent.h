#pragma once

#include <SaveLoad/SaveLoadTypeIds.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Serialization/EditContext.h>
#include <SaveData/SaveDataRequestBus.h>

namespace SaveLoad
{
    class SaveLoadComponent
        : public AZ::Component
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

        void SaveBufferToPersistentStorage();
        void LoadBufferFromPersistentStorage();
        void SaveObjectToPersistentStorage();
        void LoadObjectFromPersistentStorage(const AzFramework::LocalUserId& localUserId);

        AZStd::string testString;
        float testFloat = 0.0f;
        int testInt = 0;
        bool testBool = false;

        static const AZ::u64 testSaveDataSize = 9;
        const char* testSaveDataName = "TestSaveData";
        char testSaveData[testSaveDataSize] = {'a', 'b', 'c', '1', '2', '3', 'x', 'y', 'z'};
    };
} // namespace SaveLoad
