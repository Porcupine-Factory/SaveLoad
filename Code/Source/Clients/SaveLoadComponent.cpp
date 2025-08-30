#include <Clients/SaveLoadComponent.h>

#include <SaveData/SaveDataNotificationBus.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/Component/ComponentApplicationBus.h>

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

        if(auto bc = azrtti_cast<AZ::BehaviorContext*>(rc))
        {
            bc->EBus<SaveLoadNotificationBus>("SaveLoadNotificationBus")
                ->Handler<SaveLoadNotificationHandler>();

            bc->EBus<SaveLoadComponentRequestBus>("SaveLoadComponentRequestBus")
                ->Attribute(AZ::Script::Attributes::Scope, AZ::Script::Attributes::ScopeFlags::Common)
                ->Attribute(AZ::Script::Attributes::Module, "save")
                ->Attribute(AZ::Script::Attributes::Category, "Save Load")
                ->Event("Save Buffer To Persistent Storage", &SaveLoadComponentRequests::SaveBufferToPersistentStorage)
                ->Event("Load Buffer From Persistent Storage", &SaveLoadComponentRequests::LoadBufferFromPersistentStorage)
                ->Event("Save Object To Persistent Storage", &SaveLoadComponentRequests::SaveObjectToPersistentStorage)
                ->Event("Load Object From Persistent Storage", &SaveLoadComponentRequests::LoadObjectFromPersistentStorage)
                ->Event("Get In Editor", &SaveLoadComponentRequests::GetInEditor)
                ->Event("Set In Editor", &SaveLoadComponentRequests::SetInEditor)
                ->Event("Get Test Bool", &SaveLoadComponentRequests::GetTestBool)
                ->Event("Set Test Bool", &SaveLoadComponentRequests::SetTestBool);
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
        // Check whether the game is being ran in the O3DE editor
        AZ::ApplicationTypeQuery applicationType;
        if (auto componentApplicationRequests = AZ::Interface<AZ::ComponentApplicationRequests>::Get();
            componentApplicationRequests != nullptr)
        {
            componentApplicationRequests->QueryApplicationType(applicationType);
        }

        if (applicationType.IsEditor())
        {
            m_inEditor = true;
        }
        else
        {
            m_inEditor = false;
        }

        // Connect the handler to the request bus
        SaveLoadComponentRequestBus::Handler::BusConnect(GetEntityId());
    }

    void SaveLoadComponent::Deactivate()
    {
        // Disconnect the handler from the request bus
        SaveLoadComponentRequestBus::Handler::BusDisconnect();
    }

    void SaveLoadComponent::GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void SaveLoadComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("SaveLoadService"));
    }

    void SaveLoadComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("SaveLoadService"));
    }

    // Event Notification methods for use in scripts
    void SaveLoadComponent::OnSavedBuffer(){}
    void SaveLoadComponent::OnLoadedBuffer(){}
    void SaveLoadComponent::OnSavedObject(){}
    void SaveLoadComponent::OnLoadedObject(){}

    void SaveLoadComponent::SaveBufferToPersistentStorage()
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load gem cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

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
            else
            {
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnSavedBuffer);
            }
        };
        SaveData::SaveDataRequestBus::Broadcast(&SaveData::SaveDataRequests::SaveDataBuffer, params);
    }

    void SaveLoadComponent::LoadBufferFromPersistentStorage()
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load gem cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        SaveData::SaveDataRequests::LoadDataBufferParams params;
        params.dataBufferName = testSaveDataName;
        params.callback = [](const SaveData::SaveDataNotifications::DataBufferLoadedParams& onLoadedParams)
        {
            if (onLoadedParams.result == SaveData::SaveDataNotifications::Result::Success)
            {
                // SaveDataNotifications::DataBuffer is a shared_ptr, so you can choose to either preserve the
                // buffer (by keeping a reference to it), or just let it go out of scope so it will be deleted.
                            SaveDataNotifications::DataBuffer loadedDataBuffer = onLoadedParams.dataBuffer;
                            SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnLoadedBuffer);
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
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load gem cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Reflect the test object class (if not already done).
        AZ::SerializeContext serializeContext;
        SaveLoadComponent::Reflect(serializeContext);

        // Create a test object instance to save.
        AZStd::shared_ptr<SaveLoadComponent> saveLoadComponent = AZStd::make_shared<SaveLoadComponent>();
        saveLoadComponent->SetTestBool(GetTestBool());

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
            else
            {
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnSavedObject);
            }
        };
        SaveData::SaveDataRequests::SaveObject(params);
    }

    void SaveLoadComponent::LoadObjectFromPersistentStorage(const AzFramework::LocalUserId& localUserId = AzFramework::LocalUserIdNone)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load gem cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

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
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnLoadedObject);
            }
            else
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequests::LoadObject(params);
    }

    bool SaveLoadComponent::GetInEditor() const
    {
        return m_inEditor;
    }

    void SaveLoadComponent::SetInEditor(const bool& new_inEditor)
    {
        m_inEditor = new_inEditor;
    }

    bool SaveLoadComponent::GetTestBool() const
    {
        return testBool;
    }

    void SaveLoadComponent::SetTestBool(const bool& new_testBool)
    {
        testBool = new_testBool;
    }
} // namespace SaveLoad
