#pragma once

static constexpr auto oLocalPlayer = 0x30ee58c;// 0x310990C; // A1 ? ? ? ? 8B 54 24 28 85 C0
static constexpr auto oGameTime = 0x30e585c;// 0x3101384; //  0F 57 C0 F3 0F 5C 0D ? ? ? ? 0F 2F C1
//static constexpr auto oGameClient = 0x30eb774L;
//static constexpr auto oGameInfo = 0x30eb774L;
static constexpr auto oHeroList = 0x184ec74;// 0x186abf4;
static constexpr auto oMinionList = 0x18511fc;// 0x186cdb8;

static constexpr auto oTryRightClick = 0x5E7580; // A1 ?? ?? ?? ?? 6A 00 8B 48 24
static constexpr auto oHandleSpellInput = 0x5CF830; // 53 8B 1D ?? ?? ?? ?? 55 56 57 8B F9 85
static constexpr auto oSpoofAddress = 0x4386; // any C3 instruction
static constexpr auto oSpoofGadget = 0xEDA0F3; // ff 23 jmp dwort ptr [ebx]
static constexpr auto oGetStatTotal = 0x1DE360; // CharBasePhysical GetStatTotal((_DWORD *)(a2 + 584), 2, 1);
static constexpr auto oDoEmote = 0x5C73A0; // "evtEmoteTaunt" sub_11BFB80(&v96, EmoteWrapper); // 8B 0D ?? ?? ?? ?? 83 EC 0C 8B 49 08 53

static constexpr auto oChatClientPtr = 0x30e080c;// 0x3109E28;// 0x30fc790;
//static constexpr auto oSendChat = 0x5F49B0;// 0x5F42A0; // outdated
//static constexpr auto oPrintChat = 0x589770;
static constexpr auto oIsNotWall = 0x993EF0;// 0x9ABB50;// 0x9A9570; // [actual address in first opcode] E8 ? ? ? ? 8B 0F 0F 57 C0
static constexpr auto oIsBrush = 0x994050;// 0x9ABBB0;// 0x9A96D0; // [actual address in first opcode] E8 ? ? ? ? 8A D8 8D 44 24 18
static constexpr auto oObjectManager = 0x1851168; // FF 74 24 2C 8B 0D ? ? ? ? E8 ? ? ? ? 8B F0 // mov     ecx, dword_2431168

static constexpr auto oViewMatrix = 0x3117398; // B9 ?? ?? ?? ?? 0F 10 80 1C 02 00 00
static constexpr auto oRenderer = 0x311a238;// 0x3135148;
static constexpr auto oRendererWidth = 0xC;
static constexpr auto oRendererHeight = oRendererWidth + 0x4;

static constexpr auto oHudInstance = 0x185119c;// 0x186cd58L;
static constexpr auto oMousePos = 0x110;

//static constexpr auto oCreatePath = ; // [actual address in first opcode] E8 ? ? ? ? F3 0F 10 44 24 ? F3 0F 11 46 ? 6A 00
static constexpr auto oSmoothPath = 0x1A37A0; // under createpath
static constexpr auto oBuildNavigationPath = 0x98B780; // inside createpath

static constexpr auto oBuffManager = 0x21B8;
// todo, find a correct name
//static constexpr auto oForGetPing = 0; // 8B 35 ? ? ? ? 85 F6 74 3B 57 68 ? ? ? ? 8D 44 24 0F C6 44 24 ? ? 50 8D 44 24 14 50 8D 4E 14 E8 ? ? ? ? 8B 40 04 85 C0 75 03 8B 46 18 5F 3B 46 18 74 0E 8B 48 10 85 C9 74 07 8B 01 6A 04

//// 148/149th index in object vtable
//// push ecx
//// movzx eax, byte [ecx+offset]
//static constexpr auto oAiManager = 0x2C98;

namespace oVFunc
{
	static constexpr auto BoundingRadius = 35;
	static constexpr auto ShouldDrawHealthBar = 124;
	static constexpr auto ShouldDrawHighlight = 126;
	static constexpr auto AiManager = 149; // GetPathController

	static constexpr auto IsClone = 207;
	static constexpr auto IsLaneMinion = 210;
	static constexpr auto GetSpellBuffs = 213; // returns oBuffManager
	static constexpr auto GetSpellbook = 214;
	static constexpr auto IsMinion = 215;
	static constexpr auto IsJungleMonster = 216;
	static constexpr auto GetRespawnTimeRemaining = 219;

	static constexpr auto IsWard = 288;
}