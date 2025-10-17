
#include "SaveLoadEditorSystemComponent.h"
#include <AzCore/Serialization/SerializeContext.h>

#include <SaveLoad/SaveLoadTypeIds.h>

namespace SaveLoad
{
    AZ_COMPONENT_IMPL(
        SaveLoadEditorSystemComponent, "SaveLoadEditorSystemComponent", SaveLoadEditorSystemComponentTypeId, BaseSystemComponent);

    void SaveLoadEditorSystemComponent::Reflect(AZ::ReflectContext* context)
    {
        if (auto serializeContext = azrtti_cast<AZ::SerializeContext*>(context))
        {
            serializeContext->Class<SaveLoadEditorSystemComponent, SaveLoadSystemComponent>()->Version(0);
        }
    }

    SaveLoadEditorSystemComponent::SaveLoadEditorSystemComponent() = default;

    SaveLoadEditorSystemComponent::~SaveLoadEditorSystemComponent() = default;

    void SaveLoadEditorSystemComponent::GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided)
    {
        BaseSystemComponent::GetProvidedServices(provided);
        provided.push_back(AZ_CRC_CE("SaveLoadEditorService"));
    }

    void SaveLoadEditorSystemComponent::GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible)
    {
        BaseSystemComponent::GetIncompatibleServices(incompatible);
        incompatible.push_back(AZ_CRC_CE("SaveLoadEditorService"));
    }

    void SaveLoadEditorSystemComponent::GetRequiredServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& required)
    {
        BaseSystemComponent::GetRequiredServices(required);
    }

    void SaveLoadEditorSystemComponent::GetDependentServices([[maybe_unused]] AZ::ComponentDescriptor::DependencyArrayType& dependent)
    {
        BaseSystemComponent::GetDependentServices(dependent);
    }

    void SaveLoadEditorSystemComponent::Activate()
    {
        SaveLoadSystemComponent::Activate();
        AzToolsFramework::EditorEvents::Bus::Handler::BusConnect();
    }

    void SaveLoadEditorSystemComponent::Deactivate()
    {
        AzToolsFramework::EditorEvents::Bus::Handler::BusDisconnect();
        SaveLoadSystemComponent::Deactivate();
    }

} // namespace SaveLoad
