#include <Clients/SaveLoadComponent.h>

#include <SaveData/SaveDataNotificationBus.h>
#include <AzCore/std/smart_ptr/make_shared.h>

namespace SaveLoad
{
    using namespace SaveData;

    void SaveLoadComponent::Reflect(AZ::ReflectContext* rc)
    {
        if(auto sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<SaveLoadComponent, AZ::Component>()
                ->Version(1)
                ->Field("testString", &SaveLoadComponent::testString)
                ->Field("testFloat", &SaveLoadComponent::testFloat)
                ->Field("testInt", &SaveLoadComponent::testInt)
                ->Field("testBool", &SaveLoadComponent::testBool)
            ;
            if(AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit::Attributes;
                ec->Class<SaveLoadComponent>("Save Load",
                    "The Save Load component is used to simplify saving and loading game data")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(Category, "Save Load")
                    ->Attribute(AZ::Edit::Attributes::HelpPageURL, "https://www.youtube.com/watch?v=O7rtXNlCNQQ")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Save Types")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->DataElement(nullptr,
                        &SaveLoadComponent::testString,
                        "testString", "testString type")
                    ->DataElement(nullptr,
                        &SaveLoadComponent::testFloat,
                        "testFloat", "testFloat type")
                    ->DataElement(nullptr,
                        &SaveLoadComponent::testInt,
                        "testInt", "testInt type")
                    ->DataElement(nullptr,
                        &SaveLoadComponent::testBool,
                        "testBool", "testBool type");
            }
        }
    }

    void SaveLoadComponent::Reflect(AZ::SerializeContext& sc)
    {
        sc.Class<SaveLoadComponent, AZ::Component>()
            ->Version(1)
            ->Field("testString", &SaveLoadComponent::testString)
            ->Field("testFloat", &SaveLoadComponent::testFloat)
            ->Field("testInt", &SaveLoadComponent::testInt)
            ->Field("testBool", &SaveLoadComponent::testBool)
        ;
    }

    void SaveLoadComponent::Activate()
    {
        AZ_Printf("", "SaveLoadComponent Activated.");
        SaveBufferToPersistentStorage();
        SaveObjectToPersistentStorage();
    }

    void SaveLoadComponent::Deactivate()
    {
        return;
    }

    void SaveLoadComponent::SaveBufferToPersistentStorage()
    {
        SaveData::SaveDataRequests::SaveDataBufferParams params;
        params.dataBuffer.reset(testSaveData);
        params.dataBufferSize = testSaveDataSize;
        params.dataBufferName = testSaveDataName;
        params.callback = [](const SaveData::SaveDataNotifications::DataBufferSavedParams& onSavedParams)
        {
            if (onSavedParams.result != SaveData::SaveDataNotifications::Result::Success)
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequestBus::Broadcast(&SaveData::SaveDataRequests::SaveDataBuffer, params);
    }

    void SaveLoadComponent::LoadBufferFromPersistentStorage()
    {
        SaveData::SaveDataRequests::LoadDataBufferParams params;
        params.dataBufferName = testSaveDataName;
        params.callback = [](const SaveData::SaveDataNotifications::DataBufferLoadedParams& onLoadedParams)
        {
            if (onLoadedParams.result == SaveData::SaveDataNotifications::Result::Success)
            {
                // SaveDataNotifications::DataBuffer is a shared_ptr, so you can choose to either preserve the
                // buffer (by keeping a reference to it), or just let it go out of scope so it will be deleted.
                            SaveDataNotifications::DataBuffer loadedDataBuffer = onLoadedParams.dataBuffer;
                // Use the loaded data buffer...
            }
            else
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequestBus::Broadcast(&SaveData::SaveDataRequests::LoadDataBuffer, params);
    }

    void SaveLoadComponent::SaveObjectToPersistentStorage()
    {
        // Reflect the test object class (if not already done).
        AZ::SerializeContext serializeContext;
        SaveLoadComponent::Reflect(serializeContext);

        // Create a test object instance to save.
        AZStd::shared_ptr<SaveLoadComponent> saveLoadComponent = AZStd::make_shared<SaveLoadComponent>();

        // Setup the save data params
        SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent> params;
        params.serializableObject = saveLoadComponent;
        params.serializeContext = &serializeContext; // Omit to use the global AZ::SerializeContext instance
        params.dataBufferName = "TestSaveObject";
        params.callback = [](const SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent>& callbackParams,
                             SaveData::SaveDataNotifications::Result callbackResult)
        {
            if (callbackResult != SaveData::SaveDataNotifications::Result::Success)
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequests::SaveObject(params);
    }

    void SaveLoadComponent::LoadObjectFromPersistentStorage(const AzFramework::LocalUserId& localUserId = AzFramework::LocalUserIdNone)
    {
        // Reflect the test object class (if not already done).
        AZ::SerializeContext serializeContext;
        SaveLoadComponent::Reflect(serializeContext);

        // Create a test object to load.
        AZStd::shared_ptr<SaveLoadComponent> saveLoadComponent = AZStd::make_shared<SaveLoadComponent>();

        // Setup the load data params
        SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent> params;
        params.serializableObject = saveLoadComponent;
        params.serializeContext = &serializeContext; // Omit to use the global AZ::SerializeContext instance
        params.dataBufferName = "TestSaveObject";
        params.callback = [](const SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent>& callbackParams,
                             SaveData::SaveDataNotifications::Result callbackResult)
        {
            if (callbackResult == SaveData::SaveDataNotifications::Result::Success)
            {
                // Use the loaded data buffer...
                AZ_UNUSED(callbackParams.serializableObject);
            }
            else
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequests::LoadObject(params);
    }
} // namespace SaveLoad
