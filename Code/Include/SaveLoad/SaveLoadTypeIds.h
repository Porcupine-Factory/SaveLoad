
#pragma once

namespace SaveLoad
{
    // System Component TypeIds
    inline constexpr const char* SaveLoadSystemComponentTypeId = "{1F74FC7B-D807-4006-A58F-4E9860DBFDE5}";
    inline constexpr const char* SaveLoadEditorSystemComponentTypeId = "{75E54327-CBD4-4E42-B189-8B951C279E45}";

    // Component TypeId from SaveLoadComponent.h
    inline constexpr const char* SaveLoadComponentTypeId = "{21481543-E318-4CAC-8ECD-46E40280EDF6}";

    // Module derived classes TypeIds
    inline constexpr const char* SaveLoadModuleInterfaceTypeId = "{F57F610A-D9CB-4D28-A9AA-BF409A668133}";
    inline constexpr const char* SaveLoadModuleTypeId = "{2A211EEA-3256-4B8D-BF20-31313C34F33F}";
    // The Editor Module by default is mutually exclusive with the Client Module
    // so they use the Same TypeId
    inline constexpr const char* SaveLoadEditorModuleTypeId = SaveLoadModuleTypeId;

    // Interface TypeIds
    inline constexpr const char* SaveLoadRequestsTypeId = "{1536D5BC-50C4-405F-B206-A9404635C2AF}";
} // namespace SaveLoad
