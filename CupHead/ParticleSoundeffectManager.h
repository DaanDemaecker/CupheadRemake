#pragma once
#include "pch.h"
#include "Enums.h"
#include "Texture.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <vector>
#include <map>

class ParticleSpriteSheet;

class ParticleSoundeffectManager final
{
public:
	ParticleSoundeffectManager(int masterVolume = 8, int musicVolume = 8, int sfxVolume = 8);
	~ParticleSoundeffectManager();

	ParticleSoundeffectManager(ParticleSoundeffectManager& rhs) = delete;
	ParticleSoundeffectManager(ParticleSoundeffectManager&& rhs) = delete;

	ParticleSoundeffectManager& operator=(ParticleSoundeffectManager& rhs) = delete;
	ParticleSoundeffectManager& operator=(ParticleSoundeffectManager&& rhs) = delete;

	void Draw() const;
	void Update(float elapsedSec);

	void ChangeMasterVolume(bool up);
	int GetMasterVolume();
	void ChangeMusicVolume(bool up);
	int GetMusicVolume();
	void ChangeSFXVolume(bool up);
	int GetSFXVolume();

	enum class Music
	{
		BotanicPanic,
		InkwellIsleOne,
		Intro,
		Intro_Loop,
		Logo,
		Tutorial,
		WinScreen
	};
	void PlaySong(Music song, bool looping = true);
	void StopSong();

	enum class SFXType
	{
		//UI
		Menu_Cardflip,
		Menu_Locked,
		Menu_Move,
		Menu_Select,
		Winscreen_Chalk,
		Winscreen_Star,
		Winscreen_Tick,
		Winscreen_Timer_End,


		//Player
		Player_Damage,
		Player_Dash,
		Player_Death,
		Player_Hit,
		Player_Jump,
		Player_Land_Ground,
		Player_Parry_Axe,
		Player_Parry_Power,
		Player_Parry_Power_Full,
		Player_Parry_Slap,
		Player_Special_Attack,


		//Bullet
		BigBullet_Hit,
		Charge_Big_Death,
		Charge_Big_Spawn,
		Charge_Charged,
		Charge_Charging,
		Charge_Small_Spawn,
		Chaser_Death,
		Chaser_Loop,
		Explosion,
		Lobber_Spawn,
		PeaShooter_Death,
		PeaShooter_Loop,
		Roundabout_Spawn,
		SpreadShot_Loop,

		//BotanicPanic
		Announcer_Intro,
		Announcer_Knockout,
		Boss_Death,
		Chauncey_Chantenay_Attack,
		Chauncey_Chantenay_Beam,
		Chauncey_Chantenay_Carrot_Death,
		Chauncey_Chantenay_Rise,
		Horace_Radiche_Death,
		Horace_Radiche_Explode,
		Horace_Radiche_Intro,
		Ollie_Bulb_Crying,
		Ollie_Bulb_Happy,
		Ollie_Bulb_Rising,
		Ollie_Bulb_Tear_Death,
		Sal_Spudder_Object_Death,
		Sal_Spudder_Rising,
		Sal_Spudder_Spit,
		

		//Null
		None
	};
	void PlaySFX(SFXType type, int loops = 0);
	void StopSFX(SFXType type);
	void StopAllSFX();
	void Pause();
	void Resume();

	enum class ParticleType
	{
		//Player
		Dash_Dust,
		Hit_FX,
		Jump_Dust,
		Parry_Spark,
		Smoke_Bomb,
		Special_Dust,
		Walk_Dust,

		//Bullets
		Charge_Big_Death,
		Charge_Big_Spawn,
		Charge_Small_Death,
		Charge_Small_Spawn,
		Chaser_Death,
		Chaser_Spawn,
		Chaser_Special_Death,
		Lobber_Death,
		Lobber_Spawn,
		Lobber_Special_Death,
		Lobber_Special_Trail,
		PeaShooter_Death,
		PeaShooter_Spawn,
		PeaShooter_Special_Death,
		Roundabout_Death,
		Roundabout_Spawn,
		Roundabout_Special_Death,
		SpreadShot_Death,
		SpreadShot_Death_Range,
		SpreadShot_Spawn,
		SpreadShot_Special_Death,
		SpreadShot_Trail,
		
		//Tutorial
		Explosion,

		//Botanic Panic
		Boss_Explosion,
		Carrot_Torpedo_Death,
		DirtBall_Death,
		Horace_Radiche_Leaves,
		Horace_Radiche_Smoke,
		Potato_Spit,
		Tear_Splash,
		Tear_Splash_Pink,
		Worm_Death,

		//Null
		Null
	};

	void CreateParticle(const Point2f& pos, ParticleType type, bool facingRight=true);
	void CreateParticle(const Point2f& pos, ParticleType type, float angle);

	void InitializeParticles(Enums::Levels levelName);
	void InitializeSFX(Enums::Levels levelName);
	void InitializeSFXMenu();

	void DeleteParticles(Enums::EffectSubDivision effectSubdivision);
	void DeleteSFX(Enums::EffectSubDivision effectSubdivision);

	void LevelEnd(Enums::Levels levelName);

private:
	std::vector<ParticleSpriteSheet*> m_pParticles;

	std::map<ParticleType, ParticleEntry*> m_ParticleEntries;
	std::map<SFXType, SFXEntry*> m_pSFXEntries;

	std::map<Music, SoundStream*> m_pMusic;
	std::map<int, SFXType> m_Channels;

	const int m_MaxVolume{12};
	int m_MasterVolume;
	int m_SFXVolume;
	int m_MusicVolume;

	bool m_PlayingIntro{ false };

	void SetVolume();

	void DeleteFinishedParticles();

	void InitializeParticlesPlayer();
	void InitializeSFXPlayer();

	void InitializeParticlesBullets();
	void InitializeSFXBullets();

	void InitializeParticlesTutorial();
	void InitializeSFXTutorial();

	void InitializeParticlesBotanicPanic();
	void InitializeSFXBotanicPanic();
};

