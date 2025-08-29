
#include "SaveLoadSystemComponent.h"

#include <SaveLoad/SaveLoadTypeIds.h>

#include <AzCore/Serialization/SerializeContext.h>

namespace SaveLoad
{
    AZ_COMPONENT_IMPL(SaveLoadSystemComponent, "SaveLoadSystemComponent",
        SaveLoadSystemComponentTypeId);

    void SaveLoadSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<SaveLoadSystemComponent, AZ::Component>()
                ->Version(0)
                ;
        }
    }

    void SaveLoadSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        provided.push_back(AZ_CRC_CE("SaveLoadService"));
    }

    void SaveLoadSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        incompatible.push_back(AZ_CRC_CE("SaveLoadService"));
    }

    void SaveLoadSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
    }

    void SaveLoadSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
    }

    SaveLoadSystemComponent::SaveLoadSystemComponent()
    {
        if (SaveLoadInterface::Get() == nullptr)
        {
            SaveLoadInterface::Register(this);
        }
    }

    SaveLoadSystemComponent::~SaveLoadSystemComponent()
    {
        if (SaveLoadInterface::Get() == this)
        {
            SaveLoadInterface::Unregister(this);
        }
    }

    void SaveLoadSystemComponent::Init()
    {
    }

    void SaveLoadSystemComponent::Activate()
    {
        SaveLoadRequestBus::Handler::BusConnect();
        AZ::TickBus::Handler::BusConnect();
    }

    void SaveLoadSystemComponent::Deactivate()
    {
        AZ::TickBus::Handler::BusDisconnect();
        SaveLoadRequestBus::Handler::BusDisconnect();
    }

    void SaveLoadSystemComponent::OnTick([[maybe_unused]] float deltaTime, [[maybe_unused]] AZ::ScriptTimePoint time)
    {
    }

} // namespace SaveLoad
