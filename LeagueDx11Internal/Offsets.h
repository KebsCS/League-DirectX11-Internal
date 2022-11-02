#pragma once

// 12.12

static constexpr auto oLocalPlayer = 0x3107314;// 0x310990C; // A1 ? ? ? ? 8B 54 24 28 85 C0
static constexpr auto oGameTime = 0x31002f8;// 0x3101384; //  0F 57 C0 F3 0F 5C 0D ? ? ? ? 0F 2F C1
//static constexpr auto oGameClient = 0x30eb774L;
//static constexpr auto oGameInfo = 0x30eb774L;
static constexpr auto oHeroList = 0x1868d88;// 0x186abf4;
static constexpr auto oMinionList = 0x24B7C28;// 0x186cdb8;

static constexpr auto oTryRightClick = 0x5FB700; // A1 ?? ?? ?? ?? 6A 00 8B 48 24
//static constexpr auto oHandleSpellInput = 0x5CF830; // 53 8B 1D ?? ?? ?? ?? 55 56 57 8B F9 85

static constexpr auto oSpoofGadget = 0xF0EF33; // ff 23 jmp dword ptr [ebx]
static constexpr auto oGetStatTotal = 0x1F6870; // "CharBasePhysical" GetStatTotal((_DWORD *)(a2 + 584), 2, 1);
static constexpr auto oDoEmote = 0x61E4A0; // "evtEmoteTaunt" sub_11BFB80(&v96, EmoteWrapper); // 8B 0D ?? ?? ?? ?? 83 EC 0C 8B 49 08 53
//static constexpr auto oGetPing = 0x333590;

static constexpr auto oChatClientPtr = 0x31073B8;// 0x3109E28;// 0x30fc790;
//static constexpr auto oSendChat = 0x5F49B0;// 0x5F42A0; // outdated
//static constexpr auto oPrintChat = 0x589770;
//static constexpr auto oIsNotWall = 0x99E160;// 0x9ABB50;// 0x9A9570; // [actual address in first opcode] E8 ? ? ? ? 8B 0F 0F 57 C0
//83 EC 10 53 8B 5C 24 30
static constexpr auto oIsNotWallOffset = 0x31319D0; // the dword inside every wall function
//static constexpr auto oIsBrush = 0x99E2C0;// 0x9ABBB0;// 0x9A96D0; // [actual address in first opcode] E8 ? ? ? ? 8A D8 8D 44 24 18
static constexpr auto oObjectManager = 0x1868CF8; // 8B 0D ?? ?? ?? ?? 89 7C 24 14    //   mov     ecx, objectManager
//static constexpr auto oMissionInfo = 0x185c0cc;

// 0x996C90 todo, test this, probably ToGround

static constexpr auto oViewMatrix = 0x3136128; // B9 ?? ?? ?? ?? 0F 10 80 1C 02 00 00
static constexpr auto oRenderer = 0x3138fac;// 0x3135148;
static constexpr auto oRendererWidth = 0xC;
static constexpr auto oRendererHeight = oRendererWidth + 0x4;

static constexpr auto oHudInstance = 0x1868dac;// 0x186cd58L;
static constexpr auto oMousePos = 0x19C;

//static constexpr auto oCreatePath = ; // [actual address in first opcode] E8 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 46 ? 6A 00
static constexpr auto oSmoothPath = 0x1BC770; // under createpath
static constexpr auto oBuildNavigationPath = 0x9D3470; // inside createpath

static constexpr auto oBuffManager = 0x21B8;
// todo, find a correct name
//static constexpr auto oForGetPing = 0; // 8B 35 ? ? ? ? 85 F6 74 3B 57 68 ? ? ? ? 8D 44 24 0F C6 44 24 ? ? 50 8D 44 24 14 50 8D 4E 14 E8 ? ? ? ? 8B 40 04 85 C0 75 03 8B 46 18 5F 3B 46 18 74 0E 8B 48 10 85 C9 74 07 8B 01 6A 04

//// 148/149th index in object vtable
//// push ecx
//// movzx eax, byte [ecx+offset]
//static constexpr auto oAiManager = 0x2C98;

namespace oVFunc // TODO
{
	static constexpr auto BoundingRadius = 34;
	static constexpr auto ShouldDrawHealthBar = 126;
	static constexpr auto ShouldDrawHighlight = 128;
	static constexpr auto AiManager = 150; // GetPathController

	static constexpr auto IsClone = 210; // not sure
	static constexpr auto IsLaneMinion = 213; // // not sure
	static constexpr auto GetSpellBuffs = 216; // returns oBuffManager
	static constexpr auto GetSpellbook = 217;
	static constexpr auto IsMinion = 218; // // not sure
	static constexpr auto IsJungleMonster = 219; //// not sure
	static constexpr auto GetRespawnTimeRemaining = 222;// 219; // 83 EC 08 56 8B F1 57 8D 8E ?? ?? 00 00 E8

	static constexpr auto IsWard = 291; // not sure
}