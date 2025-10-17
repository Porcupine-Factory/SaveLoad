
#pragma once

#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <Clients/SaveLoadSystemComponent.h>

namespace SaveLoad
{
    /// System component for SaveLoad editor
    class SaveLoadEditorSystemComponent
        : public SaveLoadSystemComponent
        , protected AzToolsFramework::EditorEvents::Bus::Handler
    {
        using BaseSystemComponent = SaveLoadSystemComponent;

    public:
        AZ_COMPONENT_DECL(SaveLoadEditorSystemComponent);

        static void Reflect(AZ::ReflectContext* context);

        SaveLoadEditorSystemComponent();
        ~SaveLoadEditorSystemComponent();

    private:
        static void GetProvidedServices(AZ::ComponentDescriptor::DependencyArrayType& provided);
        static void GetIncompatibleServices(AZ::ComponentDescriptor::DependencyArrayType& incompatible);
        static void GetRequiredServices(AZ::ComponentDescriptor::DependencyArrayType& required);
        static void GetDependentServices(AZ::ComponentDescriptor::DependencyArrayType& dependent);

        // AZ::Component
        void Activate() override;
        void Deactivate() override;
    };
} // namespace SaveLoad
