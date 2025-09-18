#ifndef SAMP_HPP
#define SAMP_HPP

#include <RakHook/rakhook.hpp>


class c_chat {
public:
	c_chat*& reference() {
		return *(c_chat**)rakhook::samp_addr(0x26E8C8);
	}
	
	void add_message(unsigned long color, const char* text) {
		((void(__thiscall*)(c_chat*, unsigned long, const char*))rakhook::samp_addr(0x679F0))(this, color, text);
	}
};


class c_input {
	using cmdproc_t = void(__cdecl*)(const char*);

public:
	enum {
		MAX_CLIENT_CMDS = 144,
		MAX_CMD_LENGTH = 32,
	};

	char pad00[0xC];
	cmdproc_t command_proc[MAX_CLIENT_CMDS];
	char command_name[MAX_CLIENT_CMDS][MAX_CMD_LENGTH + 1];
	int command_count;


	c_input*& reference() {
		return *(c_input**)rakhook::samp_addr(0x26E8CC);
	}

	void add_command(const char* name, cmdproc_t cmdproc) {
		((void(__thiscall*)(c_input*, const char*, cmdproc_t)) rakhook::samp_addr(0x69000))(this, name, cmdproc);
	}

	void remove_command(std::string name) {
		for (int i = 0; i < command_count; i++) {
			if (std::string(command_name[i]) == name) {
				int needs = command_count - i;
				int clear = i;

				if (needs > 0) {
					memmove(command_name[i], command_name[i + 1], sizeof(command_name[i]) * needs);
					memmove(command_proc + i, command_proc + i + 1, sizeof(command_proc[i]) * needs);

					clear = i + needs;
				}
				memset(command_name[clear], 0, sizeof(command_name[clear]));

				command_proc[clear] = nullptr;
				command_count -= 1;
			}
		}
	}
};


#endif // SAMP_HPP