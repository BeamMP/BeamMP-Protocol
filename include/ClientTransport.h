#pragma once

#include <compare>
#include <cstdint>
#include <limits>
#include <span>

namespace bmp {

enum ClientPurpose : uint16_t {
    // ClientIdentification
    GameInfo = 0x0001,
    LauncherInfo = 0x0002,
    Error = 0x0003, // TODO: Where else is this used

    // Login
    AskForCredentials = 0x0004,
    Credentials = 0x0005,
    LoginResult = 0x0006,

    // QuickJoin
    DoJoin = 0x0007,
    JoinOk = 0x0008,
    JoinDeny = 0x0009,

    // Browsing
    ServerListRequest = 0x000a,
    ServerListResponse = 0x000b,
    Connect = 0x000c,
    Logout = 0x000d,

    // ServerIdentification
    ConnectError = 0x000e,

    // ServerAuthentication
    AuthenticationError = 0x000f,
    AuthenticationOk = 0x0010,
    PlayerRejected = 0x0011,

    // ServerModDownload
    ModSyncStatus = 0x0012,
    MapInfo = 0x0013,
    Disconnect = 0x0014,

    // ServerSessionSetup
    PlayersAndVehiclesInfo = 0x0015,
    Ready = 0x0016,

    // Playing
    Ping = 0x0011,
    VehicleSpawn = 0x0101,
    VehicleDelete = 0x0102,
    VehicleReset = 0x0103,
    VehicleEdited = 0x0104,
    VehicleCouplerChanged = 0x0105,
    SpectatorSwitched = 0x0106,
    ApplyInput = 0x0201,
    ApplyElectrics = 0x0301,
    ApplyNodes = 0x0401,
    ApplyBreakgroups = 0x0402,
    ApplyPowertrain = 0x0501,
    ApplyPosition = 0x0601,
    ChatMessage = 0x0701,
    Event = 0x0801,
    PlayerJoined = 0x0901,
    PlayerLeft = 0x0902,
    PlayerPingUpdate = 0x0903,
    Notification = 0x0a01,
    Kicked = 0x0b01,

    StateChangeLogin = 0xaa01,
    StateChangeQuickJoin = 0xaa02,
    StateChangeBrowsing = 0xaa03,
    StateChangeServerIdentification = 0xaa04,
    StateChangeServerAuthentication = 0xaa05,
    StateChangeServerModDownload = 0xaa06,
    StateChangeServerSessionSetup = 0xaa07,
    StateChangePlaying = 0xaa08,
};

enum ClientFlags : uint8_t {
    None = 0,
    MsgPack = 1 << 0,
};

struct ClientHeader {
    static constexpr size_t SERIALIZED_SIZE = 13;
    static constexpr uint32_t INVALID_PID = std::numeric_limits<uint32_t>::max();
    static constexpr uint16_t INVALID_VID = std::numeric_limits<uint16_t>::max();

    ClientFlags flags {};
    ClientPurpose purpose {};
    uint32_t pid {};
    uint16_t vid {};
    uint32_t data_size {};

    size_t deserialize_from(std::span<const uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const ClientHeader&) const = default;
};

}
