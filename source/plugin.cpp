#include "plugin.hpp"


void patch_amazing_sensfix() {
	auto amazing = reinterpret_cast<uintptr_t>(GetModuleHandleA("aggmain.asi"));

	mem::safe_set(amazing + 0x250B31, 0x90, 6);
	mem::safe_set(amazing + 0x250B8D, 0x90, 6);
	mem::safe_set(amazing + 0x250BE9, 0x90, 6);

	for (uintptr_t addr = amazing + 0x11993F0; addr <= amazing + 0x11993F8; addr += 0x4)
		*reinterpret_cast<float*>(addr) = 0.01250000019f;
}


void c_plugin::update_hooked(const decltype(update_hook)& hook) {
	static bool initialized = false;
	
	if (!initialized && c_chat().reference() != nullptr && rakhook::initialize()) {
		initialized = true;
		
		patch_amazing_sensfix();
		
		rakhook::on_receive_packet += [](Packet* packet) -> bool {
			RakNet::BitStream* bs = new RakNet::BitStream(packet->data, packet->length, false);

			uint8_t id;
			bs->Read(id);
			
			if (id == PacketEnumeration::ID_CONNECTION_REQUEST_ACCEPTED) {
				c_chat().reference()->add_message(-1, "{c43d5c}sensfix by {ffffff}waparabka{c43d5c} loaded");
				c_input().reference()->remove_command("sensfix");
			}

			delete bs;

			return true;
		};
		
		c_input().reference()->add_command("sens", [](const char* p) {
			auto sens = atoi(p);
			
			if (sens <= 0 || sens > 10) {
				c_chat().reference()->add_message(-1, "{c43d5c}[sensfix]{ffffff} -> Установите сенс от 1 до 10"); return;
			}
			*reinterpret_cast<float*>(0xB6EC1C) = (float)sens / 3200.f;
			
			c_chat().reference()->add_message(-1, std::string("{c43d5c}[sensfix]{ffffff} -> Сенса установлена на " + std::to_string(sens) + " палочку").c_str()); return;
		});
	}
	return hook.call_trampoline();
}