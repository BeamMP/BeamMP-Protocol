#pragma once

namespace bmp {

enum class State {
    Identification,
    Authentication,
    ModDownload,
    SessionSetup,
    Playing,
    Leaving
};

}
