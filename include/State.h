#pragma once

namespace bmp {

enum class State {
    None = 0,
    Identification,
    Authentication,
    ModDownload,
    SessionSetup,
    Playing,
    Leaving
};

}
