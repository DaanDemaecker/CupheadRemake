#pragma once

namespace Enums
{
	enum class Actions
	{
		Up,
		Down,
		Left,
		Right,
		Jump,
		Dash,
		Aim,
		Shoot,
		Ex,
		SwapBullets
	};

	enum class Levels
	{
		Tutorial,
		PlaneTutorial,
		BotanicPanic
	};

	enum class Character
	{
		CupHead,
		MugMan
	};

	enum class Charm
	{
		Heart,
		Coffee,
		Smoke_Bomb,
		PSugar,
		TwinHearts,
		Whetstone,
		HeartRing,
		AstralCookie,
		BrokenRelic,
		CursedRelic1,
		CursedRelic2,
		CursedRelic3,
		CursedRelic4,
		DivineRelic,
		None
	};

	enum class BulletType
	{
		PeaShooter,
		SpreadShot,
		Roundabout,
		Lobber,
		Charge,
		Chaser,
		CrackShot,
		Converge,
		TwistUp,
		none
	};

	enum class PlaneBulletType
	{
		PeaShooter,
		Mini_Bomb,
		Chalice_PeaShooter,
		Chalice_Bomb,
		Mini,
		none
	};

	enum class Buildings
	{
		Tutorial,
		ShmupTutorial,
		BotanicPanic,
		ClipJointCalammity,
		FloralFury,
		RuseOfAnOoze,
		ThreateninZeppelin,
		ForestFollies,
		TreetopTrouble,
		DieHouse,
		Shop,
		Mausoleum
	};

	enum class EffectSubDivision
	{
		Player,
		Bullet,

		Tutorial,
		BotanicPanic,

		UI
	};
}