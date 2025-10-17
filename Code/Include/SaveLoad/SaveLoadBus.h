
#pragma once

#include <SaveLoad/SaveLoadTypeIds.h>

#include <AzCore/EBus/EBus.h>
#include <AzCore/Interface/Interface.h>

namespace SaveLoad
{
    class SaveLoadRequests
    {
    public:
        AZ_RTTI(SaveLoadRequests, SaveLoadRequestsTypeId);
        virtual ~SaveLoadRequests() = default;
        // Put your public methods here
    };

    class SaveLoadBusTraits : public AZ::EBusTraits
    {
    public:
        //////////////////////////////////////////////////////////////////////////
        // EBusTraits overrides
        static constexpr AZ::EBusHandlerPolicy HandlerPolicy = AZ::EBusHandlerPolicy::Single;
        static constexpr AZ::EBusAddressPolicy AddressPolicy = AZ::EBusAddressPolicy::Single;
        //////////////////////////////////////////////////////////////////////////
    };

    using SaveLoadRequestBus = AZ::EBus<SaveLoadRequests, SaveLoadBusTraits>;
    using SaveLoadInterface = AZ::Interface<SaveLoadRequests>;

} // namespace SaveLoad
