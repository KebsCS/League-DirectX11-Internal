#pragma once

static constexpr auto oLocalPlayer = 0x310990C;
static constexpr auto oGameTime = 0x3101384;
//static constexpr auto oGameClient = 0x30eb774L;
//static constexpr auto oGameInfo = 0x30eb774L;
//static constexpr auto oHudInstance = 0x185c058L;
//static constexpr auto oRenderer = 0x3121da8L;
static constexpr auto oHeroList = 0x186abf4;
static constexpr auto oMinionList = 0x186cdb8;

static constexpr auto oChatClientPtr = 0x30fc790;
static constexpr auto oSendChat = 0x5F42A0;
static constexpr auto oPrintChat = 0x589770;
static constexpr auto oIsNotWall = 0x9A9570; // [actual address in first opcode] E8 ? ? ? ? 8B 0F 0F 57 C0
static constexpr auto oIsBrush = 0x9A96D0; // [actual address in first opcode] E8 ? ? ? ? 8A D8 8D 44 24 18

static constexpr auto oViewMatrix = 0x31322A8;
static constexpr auto oRenderer = 0x3135148;
static constexpr auto oRendererWidth = 0xC;
static constexpr auto oRendererHeight = oRendererWidth + 0x4;

static constexpr auto oHudInstance = 0x186cd58L;
//
//// 148/149th index in object vtable
//// push ecx
//// movzx eax, byte [ecx+offset]
//static constexpr auto oAiManager = 0x2C98;

namespace oVFunc
{
	static constexpr auto BoundingRadius = 35;
	static constexpr auto IsAlive = 126;
	static constexpr auto AiManager = 149;
}