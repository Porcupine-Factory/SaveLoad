
#include "SaveLoadSystemComponent.h"
#include <SaveLoad/SaveLoadTypeIds.h>
#include <SaveLoadModuleInterface.h>

namespace SaveLoad
{
    class SaveLoadModule : public SaveLoadModuleInterface
    {
    public:
        AZ_RTTI(SaveLoadModule, SaveLoadModuleTypeId, SaveLoadModuleInterface);
        AZ_CLASS_ALLOCATOR(SaveLoadModule, AZ::SystemAllocator);
    };
} // namespace SaveLoad

#if defined(O3DE_GEM_NAME)
AZ_DECLARE_MODULE_CLASS(AZ_JOIN(Gem_, O3DE_GEM_NAME), SaveLoad::SaveLoadModule)
#else
AZ_DECLARE_MODULE_CLASS(Gem_SaveLoad, SaveLoad::SaveLoadModule)
#endif
