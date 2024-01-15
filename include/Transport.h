#pragma once

#include <cstdint>
#include <span>

namespace bmp {

enum Purpose : uint16_t {
    // Identification
    ProtocolVersion = 0x0001,
    ProtocolVersionOk = 0x0002,
    ProtocolVersionBad = 0x0003,
    ClientInfo = 0x0004,
    ServerInfo = 0x0005,
    // Authentication
    PlayerPublicKey = 0x0006,
    AuthOk = 0x0007,
    AuthFailed = 0x0008,
    PlayerRejected = 0x0009,
    // ModDownload
    ModsInfo = 0x000a,
    ModRequest = 0x000b,
    ModResponse = 0x000c,
    ModRequestInvalid = 0x000d,
    ModsSyncDone = 0x000e,
    // SessionSetup
    PlayersVehiclesInfo = 0x000f,
    SessionReady = 0x0010,
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
    // Other
    StateChangeIdentification = 0xaa01,
    StateChangeAuthentication = 0xaa02,
    StateChangeModDownload = 0xaa03,
    StateChangeSessionSetup = 0xaa04,
    StateChangePlaying = 0xaa05,
    StateChangeLeaving = 0xaa06,
};

enum Flags : uint8_t {
    ZstdCompressed = 1 << 0,
};

/// Represents a header of a packet.
struct Header {
    Purpose purpose;
    Flags flags;
    uint8_t rsv;
    uint32_t size;

    size_t deserialize_from(std::span<uint8_t> span);
    size_t serialize_to(std::span<uint8_t> span);

    auto operator<=>(const Header&) const = default;
};

}
