#pragma once

namespace bmp {

enum ClientState {
    ClientIdentification = 0xC0,
    Login = 0xC1,
    QuickJoin = 0xC2,
    Browsing = 0xC3,
    ServerIdentification = 0xC4,
    ServerAuthentication = 0xC5,
    ServerModDownload = 0xC6,
    ServerSessionSetup = 0xC7,
    ServerPlaying = 0xC8, // marked as TODO in the spec?!
    ServerLeaving = 0xC9, // marked as TODO in the spec?!
};

}
