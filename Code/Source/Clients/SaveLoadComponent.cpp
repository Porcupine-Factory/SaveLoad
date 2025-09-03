#include <Clients/SaveLoadComponent.h>

#include <SaveData/SaveDataNotificationBus.h>
#include <AzCore/std/smart_ptr/make_shared.h>
#include <AzCore/Component/ComponentApplicationBus.h>
#include <AzFramework/Components/TransformComponent.h>

namespace SaveLoad
{
    using namespace SaveData;

    void SaveLoadComponent::Reflect(AZ::ReflectContext* rc)
    {
        if(auto sc = azrtti_cast<AZ::SerializeContext*>(rc))
        {
            sc->Class<SaveLoadComponent, AZ::Component>()
                ->Version(1)
                ->Field("testBool", &SaveLoadComponent::m_testBool)
            ;

            if(AZ::EditContext* ec = sc->GetEditContext())
            {
                using namespace AZ::Edit::Attributes;
                ec->Class<SaveLoadComponent>("Save Load",
                    "The Save Load component is used to simplify saving and loading game data")
                    ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                    ->Attribute(AppearsInAddComponentMenu, AZ_CRC_CE("Game"))
                    ->Attribute(Category, "Save Load")

                    ->ClassElement(AZ::Edit::ClassElements::Group, "Save Types")
                    ->Attribute(AZ::Edit::Attributes::AutoExpand, false)
                    ->DataElement(nullptr,
                        &SaveLoadComponent::m_testBool,
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
                ->Event("Save String To Persistent Storage", &SaveLoadComponentRequests::SaveStringToPersistentStorage)
                ->Event("Load String From Persistent Storage", &SaveLoadComponentRequests::LoadStringFromPersistentStorage)
                ->Event("Save This SaveLoad Component To Persistent Storage", &SaveLoadComponentRequests::SaveThisSaveLoadComponentToPersistentStorage)
                ->Event("Load This SaveLoad Component From Persistent Storage", &SaveLoadComponentRequests::LoadThisSaveLoadComponentFromPersistentStorage)
                ->Event("Save Transform Component To Persistent Storage", &SaveLoadComponentRequests::SaveTransformComponentToPersistentStorage)
                ->Event("Load Transform Component From Persistent Storage", &SaveLoadComponentRequests::LoadTransformComponentFromPersistentStorage)
                ->Event("Get Last String Save Load Filename", &SaveLoadComponentRequests::GetLastStringSaveLoadFilename)
                ->Event("Get Last This SaveLoad Component Save Load Filename", &SaveLoadComponentRequests::GetLastThisSaveLoadComponentSaveLoadFilename)
                ->Event("Get Last Transform Component Save Load Filename", &SaveLoadComponentRequests::GetLastTransformComponentSaveLoadFilename)
                ->Event("Get In Editor", &SaveLoadComponentRequests::GetInEditor)
                ->Event("Set In Editor", &SaveLoadComponentRequests::SetInEditor)
                ->Event("Get Test Bool", &SaveLoadComponentRequests::GetTestBool)
                ->Event("Set Test Bool", &SaveLoadComponentRequests::SetTestBool);
        }
    }

    void SaveLoadComponent::Reflect(AZ::SerializeContext& sc)
    {
        sc.Class<SaveLoadComponent>()
            ->Version(1)
            ->Field("testBool", &SaveLoadComponent::m_testBool)
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
    void SaveLoadComponent::OnSavedStringFile(){}
    void SaveLoadComponent::OnLoadedStringFile(const AZStd::string& loadedString){}
    void SaveLoadComponent::OnSavedThisSaveLoadComponentFile(){}
    void SaveLoadComponent::OnLoadedThisSaveLoadComponentFile(){}
    void SaveLoadComponent::OnSavedTransformComponentFile(){}
    void SaveLoadComponent::OnLoadedTransformComponentFile(){}

    void SaveLoadComponent::SaveStringToPersistentStorage(const AZStd::string& stringSaveFilename, const AZStd::string& stringToSave)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load component cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Use the default or previous filename if one wasn't specified
        if (!stringSaveFilename.empty())
        {
            m_stringSaveLoadFilename = stringSaveFilename;
        }

        SaveData::SaveDataRequests::SaveDataBufferParams params;
        const int length = stringToSave.length();
        // Allocate heap memory and construct a character array / C string from the AZStd::string that was passed in
        char* tempCString = (char*) azmalloc(sizeof(char) * length);
        for (int i = 0; i < length; i++)
        {
            tempCString[i] = stringToSave.c_str()[i];
        }
        params.dataBuffer.reset(tempCString);
        params.dataBufferSize = length;
        params.dataBufferName = m_stringSaveLoadFilename;
        params.callback = [tempCString](const SaveData::SaveDataNotifications::DataBufferSavedParams& onSavedParams)
        {
            if (onSavedParams.result != SaveData::SaveDataNotifications::Result::Success)
            {
                // Error handling
            }
            else
            {
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnSavedStringFile);
            }

            // Free up the memory that was used to save to persistent storage
            azfree(tempCString);
        };
        SaveData::SaveDataRequestBus::Broadcast(&SaveData::SaveDataRequests::SaveDataBuffer, params);
    }

    void SaveLoadComponent::LoadStringFromPersistentStorage(const AZStd::string& bufferLoadFilename)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load component cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Use the default or previous filename if one wasn't specified
        if (!bufferLoadFilename.empty())
        {
            m_stringSaveLoadFilename = bufferLoadFilename;
        }

        SaveData::SaveDataRequests::LoadDataBufferParams params;
        params.dataBufferName = m_stringSaveLoadFilename;
        params.callback = [this](const SaveData::SaveDataNotifications::DataBufferLoadedParams& onLoadedParams)
        {
            if (onLoadedParams.result == SaveData::SaveDataNotifications::Result::Success)
            {
                // SaveDataNotifications::DataBuffer is a shared_ptr, so you can choose to either preserve the
                // buffer (by keeping a reference to it), or just let it go out of scope so it will be deleted.
                AZStd::string tempString((const char*)onLoadedParams.dataBuffer.get(), onLoadedParams.dataBufferSize);
                m_loadedString = tempString;
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnLoadedStringFile, m_loadedString);
                // Use the loaded data buffer...
            }
            else
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequestBus::Broadcast(&SaveData::SaveDataRequests::LoadDataBuffer, params);
    }

    void SaveLoadComponent::SaveThisSaveLoadComponentToPersistentStorage(const AZStd::string& thisSaveLoadComponentSaveFilename)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load component cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Use the default or previous filename if one wasn't specified
        if (!thisSaveLoadComponentSaveFilename.empty())
        {
            m_thisSaveLoadComponentSaveLoadFilename = thisSaveLoadComponentSaveFilename;
        }

        // Reflect the Save Load Component class (if not already done).
        AZ::SerializeContext serializeContext;
        SaveLoadComponent::Reflect(serializeContext);

        // Use *this instance of Save Load Component to save.
        AZStd::shared_ptr<SaveLoadComponent> saveLoadComponent = AZStd::make_shared<SaveLoadComponent>(*this);

        // Setup the save data params
        SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent> params;
        params.serializableObject = saveLoadComponent;
        params.serializeContext = &serializeContext; // Omit to use the global AZ::SerializeContext instance
        params.dataBufferName = m_thisSaveLoadComponentSaveLoadFilename;
        params.callback = [](const SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent>& callbackParams,
                             SaveData::SaveDataNotifications::Result callbackResult)
        {
            if (callbackResult != SaveData::SaveDataNotifications::Result::Success)
            {
                // Error handling
            }
            else
            {
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnSavedThisSaveLoadComponentFile);
            }
        };
        SaveData::SaveDataRequests::SaveObject(params);
    }

    void SaveLoadComponent::LoadThisSaveLoadComponentFromPersistentStorage(const AZStd::string& thisSaveLoadComponentLoadFilename, const AzFramework::LocalUserId& localUserId = AzFramework::LocalUserIdNone)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load component cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Use the default or previous filename if one wasn't specified
        if (!thisSaveLoadComponentLoadFilename.empty())
        {
            m_thisSaveLoadComponentSaveLoadFilename = thisSaveLoadComponentLoadFilename;
        }

        // Use *this instance of Save Load Component to load.
        AZStd::shared_ptr<SaveLoadComponent> saveLoadComponent = AZStd::make_shared<SaveLoadComponent>(*this);

        // Setup the load data params
        SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent> params;
        params.serializableObject = saveLoadComponent;
        params.dataBufferName = m_thisSaveLoadComponentSaveLoadFilename;
        params.callback = [this](const SaveData::SaveDataRequests::SaveOrLoadObjectParams<SaveLoadComponent>& callbackParams,
                             SaveData::SaveDataNotifications::Result callbackResult)
        {
            if (callbackResult == SaveData::SaveDataNotifications::Result::Success)
            {
                // Use the loaded data buffer...
                *this = *callbackParams.serializableObject;
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnLoadedThisSaveLoadComponentFile);
            }
            else
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequests::LoadObject(params);
    }

    void SaveLoadComponent::SaveTransformComponentToPersistentStorage(const AZStd::string& transformComponentSaveFilename, const AZ::EntityId& entityIdToSaveTransformComponent)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load component cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Use the default or previous filename if one wasn't specified
        if (!transformComponentSaveFilename.empty())
        {
            m_transformComponentSaveLoadFilename = transformComponentSaveFilename;
        }

        // Check if the entityId that was given is valid
        if (!entityIdToSaveTransformComponent.IsValid())
        {
            AZ_Warning("Save Load Component", false, "The EntityId given to SaveTransformComponentToPersistentStorage() is invalid.");
            return;
        }

        // Get a pointer to the transform component of the entity that's passed in
        AZ::Entity* entityPtrToSaveTransformComponent = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(entityPtrToSaveTransformComponent, &AZ::ComponentApplicationRequests::FindEntity, entityIdToSaveTransformComponent);
        AzFramework::TransformComponent* transformComponentPtr = entityPtrToSaveTransformComponent->FindComponent<AzFramework::TransformComponent>();

        // Use *transformComponentPtr instance of Transform Component to save.
        AZStd::shared_ptr<AzFramework::TransformComponent> transformComponentSharedPtr = AZStd::make_shared<AzFramework::TransformComponent>(*transformComponentPtr);

        // Setup the save data params
        SaveData::SaveDataRequests::SaveOrLoadObjectParams<AzFramework::TransformComponent> params;
        params.serializableObject = transformComponentSharedPtr;
        params.dataBufferName = m_transformComponentSaveLoadFilename;
        params.callback = [](const SaveData::SaveDataRequests::SaveOrLoadObjectParams<AzFramework::TransformComponent>& callbackParams,
                             SaveData::SaveDataNotifications::Result callbackResult)
        {
            if (callbackResult != SaveData::SaveDataNotifications::Result::Success)
            {
                // Error handling
            }
            else
            {
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnSavedTransformComponentFile);
            }
        };
        SaveData::SaveDataRequests::SaveObject(params);
    }

    void SaveLoadComponent::LoadTransformComponentFromPersistentStorage(const AZStd::string& transformComponentLoadFilename, const AZ::EntityId& entityIdToLoadTransformComponent, const AzFramework::LocalUserId& localUserId = AzFramework::LocalUserIdNone)
    {
        if (m_inEditor)
        {
            AZ_Warning("Save Load Component", false, "Editor environment detected, the Save Load component cannot be used in the editor, only with the *.GameLauncher.");
            return;
        }

        // Use the default or previous filename if one wasn't specified
        if (!transformComponentLoadFilename.empty())
        {
            m_transformComponentSaveLoadFilename = transformComponentLoadFilename;
        }

        // Check if the entityId that was given is valid
        if (!entityIdToLoadTransformComponent.IsValid())
        {
            AZ_Warning("Save Load Component", false, "The EntityId given to LoadTransformComponentFromPersistentStorage() is invalid.");
            return;
        }

        // Get a pointer to the transform component of the entity that's passed in
        AZ::Entity* entityPtrToLoadTransformComponent = nullptr;
        AZ::ComponentApplicationBus::BroadcastResult(entityPtrToLoadTransformComponent, &AZ::ComponentApplicationRequests::FindEntity, entityIdToLoadTransformComponent);
        AzFramework::TransformComponent* transformComponentPtr = entityPtrToLoadTransformComponent->FindComponent<AzFramework::TransformComponent>();

        // Use *transformComponentPtr instance of Transform Component to load.
        AZStd::shared_ptr<AzFramework::TransformComponent> transformComponentSharedPtr = AZStd::make_shared<AzFramework::TransformComponent>(*transformComponentPtr);

        // Setup the load data params
        SaveData::SaveDataRequests::SaveOrLoadObjectParams<AzFramework::TransformComponent> params;
        params.serializableObject = transformComponentSharedPtr;
        params.dataBufferName = m_transformComponentSaveLoadFilename;
        params.callback = [transformComponentPtr](const SaveData::SaveDataRequests::SaveOrLoadObjectParams<AzFramework::TransformComponent>& callbackParams,
                             SaveData::SaveDataNotifications::Result callbackResult) mutable
        {
            if (callbackResult == SaveData::SaveDataNotifications::Result::Success)
            {
                // Use the loaded data buffer...
                // Since the copy assignment operator for the TransformComponent is implicitly deleted, use the various set and get methods instead assigning it
                transformComponentPtr->SetParent(callbackParams.serializableObject->GetParentId());
                transformComponentPtr->SetLocalTM(callbackParams.serializableObject->GetLocalTM());
                transformComponentPtr->SetWorldTM(callbackParams.serializableObject->GetWorldTM());
                SaveLoadNotificationBus::Broadcast(&SaveLoadNotificationBus::Events::OnLoadedTransformComponentFile);
            }
            else
            {
                // Error handling
            }
        };
        SaveData::SaveDataRequests::LoadObject(params);
    }

    AZStd::string SaveLoadComponent::GetLastStringSaveLoadFilename() const
    {
        return m_stringSaveLoadFilename;
    }

    AZStd::string SaveLoadComponent::GetLastThisSaveLoadComponentSaveLoadFilename() const
    {
        return m_thisSaveLoadComponentSaveLoadFilename;
    }

    AZStd::string SaveLoadComponent::GetLastTransformComponentSaveLoadFilename() const
    {
        return m_transformComponentSaveLoadFilename;
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
        return m_testBool;
    }

    void SaveLoadComponent::SetTestBool(const bool& new_testBool)
    {
        m_testBool = new_testBool;
    }
} // namespace SaveLoad
