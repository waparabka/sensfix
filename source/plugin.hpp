#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "samp.hpp"

using namespace std::placeholders;


class c_plugin {
public:
    c_plugin() = default;
    ~c_plugin() { rakhook::destroy(); };

    kthook::kthook_simple<void(__cdecl*)()> update_hook { reinterpret_cast<void*>(0x561B10), std::bind(&c_plugin::update_hooked, this, _1) };
    void update_hooked(const decltype(update_hook)& hook);

} _plugin;


#endif // PLUGIN_HPP