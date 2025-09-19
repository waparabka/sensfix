#include "plugin.hpp"


void patch_amazing_sensfix() {
	auto amazing = reinterpret_cast<uintptr_t>(GetModuleHandleA("aggmain.asi"));
	auto sizeof_amazing = reinterpret_cast<IMAGE_NT_HEADERS*>(amazing + reinterpret_cast<IMAGE_DOS_HEADER*>(amazing)->e_lfanew)->OptionalHeader.SizeOfImage;
	
	auto sensfix_addr = mem::find_pattern(amazing, sizeof_amazing,
		"\x55\x8b\xec\x6a\x00\x68\x00\x00\x00\x00\x64\xa1\x00\x00\x00\x00\x50\x81\xec\x00\x00\x00\x00\xa1\x00\x00\x00\x00\x33\xc5\x89\x45\x00\x56\x57\x50\x8d\x45\x00\x64\xa3\x00\x00\x00\x00\x8b\xf9\xc6\x85\x00\x00\x00\x00\x00\x8d\x85",
		"xxxx?x????xx????xxx????x????xxxx?xxxxx?xx????xxxx?????xx");
	
	mem::safe_set(sensfix_addr + 0x749, 0x90, 6);
	mem::safe_set(sensfix_addr + 0x691, 0x90, 6);
	mem::safe_set(sensfix_addr + 0x6ED, 0x90, 6);

	// + 0x749 0x250ad9 global
	// + 0x691 0x250a21 aiming
	// + 0x6ED 0x250a7d sniper

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
			
			c_chat().reference()->add_message(-1, std::string("{c43d5c}[sensfix]{ffffff} -> Сенс установлен на " + std::to_string(sens) + " палочку").c_str()); return;
		});
	}
	return hook.call_trampoline();
}
