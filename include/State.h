#pragma once

namespace bmp {

enum class State {
    None = 0,
    Identification = 1,
    Authentication = 2,
    ModDownload = 3,
    SessionSetup = 4,
    Playing = 5,
    Leaving = 6
};

}
