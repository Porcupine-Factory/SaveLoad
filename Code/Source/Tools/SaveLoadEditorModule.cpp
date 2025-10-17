
#include "SaveLoadEditorSystemComponent.h"
#include <SaveLoad/SaveLoadTypeIds.h>
#include <SaveLoadModuleInterface.h>

namespace SaveLoad
{
    class SaveLoadEditorModule : public SaveLoadModuleInterface
    {
    public:
        AZ_RTTI(SaveLoadEditorModule, SaveLoadEditorModuleTypeId, SaveLoadModuleInterface);
        AZ_CLASS_ALLOCATOR(SaveLoadEditorModule, AZ::SystemAllocator);

        SaveLoadEditorModule()
        {
            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and
            // EditContext. This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(
                m_descriptors.end(),
                {
                    SaveLoadEditorSystemComponent::CreateDescriptor(),
                });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
                azrtti_typeid<SaveLoadEditorSystemComponent>(),
            };
        }
    };
} // namespace SaveLoad

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME, _Editor), SaveLoad::SaveLoadEditorModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_SaveLoad_Editor, SaveLoad::SaveLoadEditorModule)
#endif
