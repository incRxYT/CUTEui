#include <Geode/Geode.hpp>

using namespace geode::prelude;

// CUTEui — soft pastel UI customization mod
// Features: progress bar, player trail, percentage display, UI skin
// All features are user-controlled via Geode settings

$on_mod(Loaded) {
    log::info("CUTEui v{} loaded!", Mod::get()->getVersion().toString());
}