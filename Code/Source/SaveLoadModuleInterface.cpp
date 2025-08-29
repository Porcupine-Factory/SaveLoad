
#include "SaveLoadModuleInterface.h"
#include <AzCore/Memory/Memory.h>

#include <SaveLoad/SaveLoadTypeIds.h>

#include <Clients/SaveLoadSystemComponent.h>
#include <Clients/SaveLoadComponent.h>

namespace SaveLoad
{
    AZ_TYPE_INFO_WITH_NAME_IMPL(SaveLoadModuleInterface,
        "SaveLoadModuleInterface", SaveLoadModuleInterfaceTypeId);
    AZ_RTTI_NO_TYPE_INFO_IMPL(SaveLoadModuleInterface, AZ::Module);
    AZ_CLASS_ALLOCATOR_IMPL(SaveLoadModuleInterface, AZ::SystemAllocator);

    SaveLoadModuleInterface::SaveLoadModuleInterface()
    {
        // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
        // Add ALL components descriptors associated with this gem to m_descriptors.
        // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
        // This happens through the [MyComponent]::Reflect() function.
        m_descriptors.insert(m_descriptors.end(), {
            SaveLoadSystemComponent::CreateDescriptor(),
            SaveLoadComponent::CreateDescriptor()
            });
    }

    AZ::ComponentTypeList SaveLoadModuleInterface::GetRequiredSystemComponents() const
    {
        return AZ::ComponentTypeList{
            azrtti_typeid<SaveLoadSystemComponent>(),
        };
    }
} // namespace SaveLoad
