#pragma once

static constexpr auto oLocalPlayer = 0x3109D4C;// 0x310990C;
static constexpr auto oGameTime = 0x3101314;// 0x3101384;
//static constexpr auto oGameClient = 0x30eb774L;
//static constexpr auto oGameInfo = 0x30eb774L;
//static constexpr auto oHudInstance = 0x185c058L;
//static constexpr auto oRenderer = 0x3121da8L;
static constexpr auto oHeroList = 0x186ab74;// 0x186abf4;
static constexpr auto oMinionList = 0x186cdfc;// 0x186cdb8;

static constexpr auto oChatClientPtr = 0x30fc718;// 0x3109E28;// 0x30fc790;
static constexpr auto oSendChat = 0x5F49B0;// 0x5F42A0;
//static constexpr auto oPrintChat = 0x589770;
static constexpr auto oIsNotWall = 0x9ABB10;// 0x9ABB50;// 0x9A9570; // [actual address in first opcode] E8 ? ? ? ? 8B 0F 0F 57 C0
static constexpr auto oIsBrush = 0x9ABC70;// 0x9ABBB0;// 0x9A96D0; // [actual address in first opcode] E8 ? ? ? ? 8A D8 8D 44 24 18
static constexpr auto oObjectManager = 0x186CD64;// 0x186CD20;

static constexpr auto oViewMatrix = 0x3132A30; // 0x31322A8;
static constexpr auto oRenderer = 0x31358d0;// 0x3135148;
static constexpr auto oRendererWidth = 0xC;
static constexpr auto oRendererHeight = oRendererWidth + 0x4;

static constexpr auto oHudInstance = 0x186CD9C;// 0x186cd58L;
static constexpr auto oMousePos = 0x110;

static constexpr auto oCreatePath = 0x169C20; // [actual address in first opcode] E8 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 46 ? 6A 00
static constexpr auto oSmoothPath = 0x19FB90;
static constexpr auto oBuildNavigationPath = 0x9A3380;

static constexpr auto oBuffManager = 0x21B8;
// todo, find a correct name
static constexpr auto oForGetPing = 0x30FC6CC; // 8B 35 ? ? ? ? 85 F6 74 3B 57 68 ? ? ? ? 8D 44 24 0F C6 44 24 ? ? 50 8D 44 24 14 50 8D 4E 14 E8 ? ? ? ? 8B 40 04 85 C0 75 03 8B 46 18 5F 3B 46 18 74 0E 8B 48 10 85 C9 74 07 8B 01 6A 04
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