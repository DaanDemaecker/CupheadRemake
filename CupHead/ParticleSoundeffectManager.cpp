#include "pch.h"
#include "ParticleSoundeffectManager.h"
#include "ParticleSpriteSheet.h"
#include <iostream>

ParticleSoundeffectManager::ParticleSoundeffectManager(int masterVolume, int musicVolume, int sfxVolume)
	:m_MasterVolume{masterVolume}, m_MusicVolume{musicVolume}, m_SFXVolume{sfxVolume}
{
	std::string textPath{ "./resources/sounds/Music/" };

	m_pMusic[Music::BotanicPanic] = new SoundStream{ textPath + "BotanicPanic.wav"};
	m_pMusic[Music::InkwellIsleOne] = new SoundStream{ textPath + "InkwellIsleOne.wav" };
	m_pMusic[Music::Intro] = new SoundStream{ textPath + "Intro.wav" };
	m_pMusic[Music::Intro_Loop] = new SoundStream{ textPath + "Intro_Loop.wav" };
	m_pMusic[Music::Logo] = new SoundStream{ textPath + "Logo.wav" };
	m_pMusic[Music::Tutorial] = new SoundStream{ textPath + "Tutorial.wav" }; 
	m_pMusic[Music::WinScreen] = new SoundStream{ textPath + "VictoryScreen.wav" };

	SetVolume();
}

ParticleSoundeffectManager::~ParticleSoundeffectManager()
{
	for (int i{}; i < m_pParticles.size(); i++)
	{
		delete m_pParticles[i];
		m_pParticles[i] = nullptr;
	}
	m_pParticles.clear();

	for (std::pair<Music, SoundStream*> pair : m_pMusic)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pMusic.clear();


	for (std::pair<SFXType, SFXEntry*> pair : m_pSFXEntries)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_pSFXEntries.clear();


	for (std::pair<ParticleType, ParticleEntry*> pair : m_ParticleEntries)
	{
		delete pair.second;
		pair.second = nullptr;
	}
	m_ParticleEntries.clear();
}

void ParticleSoundeffectManager::Draw() const
{
	for (int i{}; i < m_pParticles.size(); i++)
	{
		m_pParticles[i]->Draw();

	}
}

void ParticleSoundeffectManager::Update(float elapsedSec)
{
	DeleteFinishedParticles();

	for (int i{}; i < m_pParticles.size(); i++)
	{
		m_pParticles[i]->Update(elapsedSec);
	}



	if (m_PlayingIntro && !SoundStream::IsPlaying())
	{
		PlaySong(Music::Intro_Loop);
		m_PlayingIntro = false;
	}
}

void ParticleSoundeffectManager::ChangeMasterVolume(bool up)
{
	if (up && m_MasterVolume < m_MaxVolume)m_MasterVolume++;
	else if(!up && m_MasterVolume > 0) m_MasterVolume--;

	SoundStream::SetVolume(m_MasterVolume * m_MusicVolume);
	for (std::pair<SFXType, SFXEntry*> pair: m_pSFXEntries)
	{
		pair.second->pSoundEffect->SetVolume(m_MasterVolume * m_SFXVolume);
	}
}

int ParticleSoundeffectManager::GetMasterVolume()
{
	return m_MasterVolume;
}

void ParticleSoundeffectManager::ChangeMusicVolume(bool up)
{
	if (up && m_MusicVolume < m_MaxVolume)m_MusicVolume++;
	else if (!up && m_MusicVolume > 0) m_MusicVolume--;

	SoundStream::SetVolume(m_MasterVolume * m_MusicVolume);

}

int ParticleSoundeffectManager::GetMusicVolume()
{
	return m_MusicVolume;
}

void ParticleSoundeffectManager::ChangeSFXVolume(bool up)
{
	if (up && m_SFXVolume < m_MaxVolume)m_SFXVolume++;
	else if (!up && m_SFXVolume > 0) m_SFXVolume--;

	for (std::pair<SFXType, SFXEntry*> pair : m_pSFXEntries)
	{
		pair.second->pSoundEffect->SetVolume(m_MasterVolume * m_SFXVolume);
	}
}

int ParticleSoundeffectManager::GetSFXVolume()
{
	return m_SFXVolume;
}

void ParticleSoundeffectManager::PlaySong(Music song, bool looping)
{
	if (song == Music::Intro)m_PlayingIntro = true;
	m_pMusic[song]->Play(looping);
}

void ParticleSoundeffectManager::StopSong()
{
	SoundStream::Stop();
}

void ParticleSoundeffectManager::PlaySFX(SFXType type, int loops)
{
	if (type != SFXType::None)
	{
		m_pSFXEntries[type]->pSoundEffect->Play(loops);
		m_Channels[m_pSFXEntries[type]->pSoundEffect->GetChannel()] = type;
	}
}

void ParticleSoundeffectManager::StopSFX(SFXType type)
{
	if(m_Channels[m_pSFXEntries[type]->pSoundEffect->GetChannel()] == type)m_pSFXEntries[type]->pSoundEffect->Stop();
}

void ParticleSoundeffectManager::StopAllSFX()
{
	SoundEffect::StopAll();
}

void ParticleSoundeffectManager::Pause()
{
	SoundEffect::PauseAll();
}

void ParticleSoundeffectManager::Resume()
{
	SoundEffect::ResumeAll();
}

void ParticleSoundeffectManager::SetVolume()
{
	SoundStream::SetVolume(m_MasterVolume * m_MusicVolume);
	for (std::pair<SFXType, SFXEntry*> pair : m_pSFXEntries)
	{
		pair.second->pSoundEffect->SetVolume(m_MasterVolume * m_SFXVolume);
	}
}

void ParticleSoundeffectManager::DeleteFinishedParticles()
{
	int particleAmount{ int(m_pParticles.size()) };

	for (int i{}; i < particleAmount; i++)
	{
		if (m_pParticles[i]->GetFinishedCycle())
		{
			delete m_pParticles[i];
			m_pParticles[i] = nullptr;
			if (--particleAmount > 0)
			{
				m_pParticles[i] = m_pParticles[m_pParticles.size() - 1];
				i--;
			}
			m_pParticles.pop_back();
		}
	}
}

void ParticleSoundeffectManager::CreateParticle(const Point2f& pos, ParticleType type, bool facingRight)
{
	if (type != ParticleType::Null)
	{
		m_pParticles.push_back(new ParticleSpriteSheet{ m_ParticleEntries[type], pos, facingRight });
	}
}

void ParticleSoundeffectManager::CreateParticle(const Point2f& pos, ParticleType type, float angle)
{
	if (type != ParticleType::Null)
	{
		m_pParticles.push_back(new ParticleSpriteSheet{ m_ParticleEntries[type], pos, angle });
	}
}


void ParticleSoundeffectManager::LevelEnd(Enums::Levels levelName)
{
	DeleteParticles(Enums::EffectSubDivision::Player);
	DeleteParticles(Enums::EffectSubDivision::Bullet);

	DeleteSFX(Enums::EffectSubDivision::Player);
	DeleteSFX(Enums::EffectSubDivision::Bullet);

	switch (levelName)
	{
	case Enums::Levels::Tutorial:
	case Enums::Levels::PlaneTutorial:
		DeleteParticles(Enums::EffectSubDivision::Tutorial);
		DeleteSFX(Enums::EffectSubDivision::Tutorial);
		break;
	case Enums::Levels::BotanicPanic:
		DeleteParticles(Enums::EffectSubDivision::BotanicPanic);
		DeleteSFX(Enums::EffectSubDivision::BotanicPanic);
		break;
	default:
		break;
	}
}

void ParticleSoundeffectManager::DeleteParticles(Enums::EffectSubDivision effectSubdivision)
{
	std::vector<ParticleType> hold;

	for (const std::pair<ParticleType, ParticleEntry*> &pair : m_ParticleEntries)
	{
		if (pair.second->subdivision == effectSubdivision)
		{
			hold.push_back(pair.first);
		}
	}

	for (int i{}; i < hold.size(); i++)
	{
		delete m_ParticleEntries[hold[i]];
		m_ParticleEntries.erase(hold[i]);
	}

}

void ParticleSoundeffectManager::DeleteSFX(Enums::EffectSubDivision effectSubdivision)
{
	std::vector<SFXType> hold;

	for (const std::pair<SFXType, SFXEntry*>& pair : m_pSFXEntries)
	{
		if (pair.second->subdivision == effectSubdivision)
		{
			hold.push_back(pair.first);
		}
	}

	for (int i{}; i < hold.size(); i++)
	{
		delete m_pSFXEntries[hold[i]];
		m_pSFXEntries.erase(hold[i]);
	}
}

void ParticleSoundeffectManager::InitializeParticles(Enums::Levels levelName)
{
	InitializeParticlesPlayer();
	InitializeParticlesBullets();
	switch (levelName)
	{
	case Enums::Levels::Tutorial:
	case Enums::Levels::PlaneTutorial:
		InitializeParticlesTutorial();
		break;
	case Enums::Levels::BotanicPanic:
		InitializeParticlesBotanicPanic();
		break;
	default:
		break;
	}
}

void ParticleSoundeffectManager::InitializeSFX(Enums::Levels levelName)
{
	InitializeSFXPlayer();
	InitializeSFXBullets();
	switch (levelName)
	{
	case Enums::Levels::BotanicPanic:
		InitializeSFXBotanicPanic();
		break;
	default:
		break;
	}
}

void ParticleSoundeffectManager::InitializeSFXMenu()
{
	std::string textPath{ "./Resources/sounds/Effects/UI/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::UI };

	m_pSFXEntries[SFXType::Menu_Cardflip] = new SFXEntry{ subdivision, textPath + "Menu_Cardflip.wav" };
	m_pSFXEntries[SFXType::Menu_Locked] = new SFXEntry{ subdivision, textPath + "Menu_Locked.wav" };
	m_pSFXEntries[SFXType::Menu_Move] = new SFXEntry{ subdivision, textPath + "Menu_Move.wav" };
	m_pSFXEntries[SFXType::Menu_Select] = new SFXEntry{ subdivision, textPath + "Menu_Select.wav" };
	m_pSFXEntries[SFXType::Winscreen_Chalk] = new SFXEntry{ subdivision, textPath + "Winscreen_Chalk.wav" };
	m_pSFXEntries[SFXType::Winscreen_Star] = new SFXEntry{ subdivision, textPath + "Winscreen_Star.wav" };
	m_pSFXEntries[SFXType::Winscreen_Tick] = new SFXEntry{ subdivision, textPath + "Winscreen_Tick.wav" };
	m_pSFXEntries[SFXType::Winscreen_Timer_End] = new SFXEntry{ subdivision, textPath + "Winscreen_Timer_End.wav" };

	SetVolume();
}

void ParticleSoundeffectManager::InitializeParticlesPlayer()
{
	std::string textPath{ "./resources/images/player/effects/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::Player };

	m_ParticleEntries[ParticleType::Dash_Dust] = new ParticleEntry{ subdivision, textPath + "Dash_Dust.png", 7, 2, 14};
	m_ParticleEntries[ParticleType::Hit_FX] = new ParticleEntry{ subdivision, textPath + "Hit_FX.png", 5, 2, 9 };
	m_ParticleEntries[ParticleType::Jump_Dust] = new ParticleEntry{ subdivision, textPath + "Jump_Dust.png", 7, 2, 14 };
	m_ParticleEntries[ParticleType::Parry_Spark] = new ParticleEntry{ subdivision, textPath + "Parry_Spark.png", 5, 2, 9 };
	m_ParticleEntries[ParticleType::Smoke_Bomb] = new ParticleEntry{ subdivision, textPath + "Smoke_Bomb.png", 5, 3, 13 };
	m_ParticleEntries[ParticleType::Special_Dust] = new ParticleEntry{ subdivision, textPath + "Special_Dust.png", 7, 2, 14 };
	m_ParticleEntries[ParticleType::Walk_Dust] = new ParticleEntry{ subdivision, textPath + "Walk_Dust.png", 19, 1, 19 };
}

void ParticleSoundeffectManager::InitializeSFXPlayer()
{
	std::string textPath{ "./Resources/sounds/Effects/Player/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::Player };

	m_pSFXEntries[SFXType::Player_Damage] = new SFXEntry{ subdivision, textPath + "Player_Damage.wav" };
	m_pSFXEntries[SFXType::Player_Dash] = new SFXEntry{ subdivision, textPath + "Player_Dash.wav" };
	m_pSFXEntries[SFXType::Player_Death] = new SFXEntry{ subdivision, textPath + "Player_Death.wav" };
	m_pSFXEntries[SFXType::Player_Hit] = new SFXEntry{ subdivision, textPath + "Player_Hit.wav" };
	m_pSFXEntries[SFXType::Player_Jump] = new SFXEntry{ subdivision, textPath + "Player_Jump.wav" };
	m_pSFXEntries[SFXType::Player_Land_Ground] = new SFXEntry{ subdivision, textPath + "Player_Land_Ground.wav" };
	m_pSFXEntries[SFXType::Player_Parry_Axe] = new SFXEntry{ subdivision, textPath + "Player_Parry_Axe.wav" };
	m_pSFXEntries[SFXType::Player_Parry_Power] = new SFXEntry{ subdivision, textPath + "Player_Parry_Power.wav" };
	m_pSFXEntries[SFXType::Player_Parry_Power_Full] = new SFXEntry{ subdivision, textPath + "Player_Parry_Power_Full.wav" };
	m_pSFXEntries[SFXType::Player_Parry_Slap] = new SFXEntry{ subdivision, textPath + "Player_Parry_Slap.wav" };
	m_pSFXEntries[SFXType::Player_Special_Attack] = new SFXEntry{ subdivision, textPath + "Player_Special_Attack.wav" };

	SetVolume();
}

void ParticleSoundeffectManager::InitializeParticlesBullets()
{
	std::string textPath{ "./resources/images/player/bullets/effects/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::Bullet };

	m_ParticleEntries[ParticleType::Charge_Big_Death] = new ParticleEntry{ subdivision, textPath + "Charge_Big_Death.png", 7, 2, 13 };
	m_ParticleEntries[ParticleType::Charge_Big_Spawn] = new ParticleEntry{ subdivision, textPath + "Charge_Big_Spawn.png", 3, 3, 9 };
	m_ParticleEntries[ParticleType::Charge_Small_Death] = new ParticleEntry{ subdivision, textPath + "Charge_Small_Death.png", 6, 1, 6 };
	m_ParticleEntries[ParticleType::Charge_Small_Spawn] = new ParticleEntry{ subdivision, textPath + "Charge_Small_Spawn.png", 4, 1, 4, 1/20.f };
	m_ParticleEntries[ParticleType::Chaser_Death] = new ParticleEntry{ subdivision, textPath + "Chaser_Death.png", 4, 1, 4 };
	m_ParticleEntries[ParticleType::Chaser_Spawn] = new ParticleEntry{ subdivision, textPath + "Chaser_Spawn.png", 4, 1, 4, 1/20.f };
	m_ParticleEntries[ParticleType::Chaser_Special_Death] = new ParticleEntry{ subdivision, textPath + "Chaser_Special_Death.png", 4, 1, 4 };
	m_ParticleEntries[ParticleType::Lobber_Death] = new ParticleEntry{ subdivision, textPath + "Lobber_Death.png", 6, 1, 6 };
	m_ParticleEntries[ParticleType::Lobber_Spawn] = new ParticleEntry{ subdivision, textPath + "Lobber_Spawn.png", 4, 1, 4, 1/20.f };
	m_ParticleEntries[ParticleType::Lobber_Special_Death] = new ParticleEntry{ subdivision, textPath + "Lobber_Special_Death.png", 5, 3, 14 };
	m_ParticleEntries[ParticleType::Lobber_Special_Trail] = new ParticleEntry{ subdivision, textPath + "Lobber_Special_Trail.png", 5, 2, 10 };
	m_ParticleEntries[ParticleType::PeaShooter_Death] = new ParticleEntry{ subdivision, textPath + "PeaShooter_Death.png", 6, 1, 6 };
	m_ParticleEntries[ParticleType::PeaShooter_Spawn] = new ParticleEntry{ subdivision, textPath + "PeaShooter_Spawn.png", 4, 1, 4, 1/20.f };
	m_ParticleEntries[ParticleType::PeaShooter_Special_Death] = new ParticleEntry{ subdivision, textPath + "PeaShooter_Special_Death.png", 5, 2, 9 };
	m_ParticleEntries[ParticleType::Roundabout_Death] = new ParticleEntry{ subdivision, textPath + "Roundabout_Death.png", 6, 1, 6 };
	m_ParticleEntries[ParticleType::Roundabout_Spawn] = new ParticleEntry{ subdivision, textPath + "Roundabout_Spawn.png", 4, 1, 4, 1/20.f };
	m_ParticleEntries[ParticleType::Roundabout_Special_Death] = new ParticleEntry{ subdivision, textPath + "Roundabout_Special_Death.png", 4, 2, 8 };
	m_ParticleEntries[ParticleType::SpreadShot_Death] = new ParticleEntry{ subdivision, textPath + "SpreadShot_Death.png", 3, 1, 3 };
	m_ParticleEntries[ParticleType::SpreadShot_Death_Range] = new ParticleEntry{ subdivision, textPath + "SpreadShot_Death_Range.png", 5, 1, 5 };
	m_ParticleEntries[ParticleType::SpreadShot_Spawn] = new ParticleEntry{ subdivision, textPath + "SpreadShot_Spawn.png", 4, 1, 4, 1/20.f };
	m_ParticleEntries[ParticleType::SpreadShot_Special_Death] = new ParticleEntry{ subdivision, textPath + "SpreadShot_Special_Death.png", 3, 2, 6 };
	m_ParticleEntries[ParticleType::SpreadShot_Trail] = new ParticleEntry{ subdivision, textPath + "SpreadShot_Trail.png", 9, 2, 17 };
}

void ParticleSoundeffectManager::InitializeSFXBullets()
{
	std::string textPath{ "./Resources/sounds/Effects/bullets/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::Bullet };

	m_pSFXEntries[SFXType::BigBullet_Hit] = new SFXEntry{ subdivision, textPath + "BigBullet_Hit.wav" };
	m_pSFXEntries[SFXType::Charge_Big_Death] = new SFXEntry{ subdivision, textPath + "Charge_Big_Death.wav" };
	m_pSFXEntries[SFXType::Charge_Big_Spawn] = new SFXEntry{ subdivision, textPath + "Charge_Big_Spawn.wav" };
	m_pSFXEntries[SFXType::Charge_Charged] = new SFXEntry{ subdivision, textPath + "Charge_Charged.wav" };
	m_pSFXEntries[SFXType::Charge_Charging] = new SFXEntry{ subdivision, textPath + "Charge_Charging.wav" };
	m_pSFXEntries[SFXType::Charge_Small_Spawn] = new SFXEntry{ subdivision, textPath + "Charge_Small_Spawn.wav" };
	m_pSFXEntries[SFXType::Chaser_Death] = new SFXEntry{ subdivision, textPath + "Chaser_Death.wav" };
	m_pSFXEntries[SFXType::Chaser_Loop] = new SFXEntry{ subdivision, textPath + "Chaser_Loop.wav" };
	m_pSFXEntries[SFXType::Explosion] = new SFXEntry{ subdivision, textPath + "Explosion.wav" };
	m_pSFXEntries[SFXType::Lobber_Spawn] = new SFXEntry{ subdivision, textPath + "Lobber_Spawn.wav" };
	m_pSFXEntries[SFXType::PeaShooter_Death] = new SFXEntry{ subdivision, textPath + "PeaShooter_Death.wav" };
	m_pSFXEntries[SFXType::PeaShooter_Loop] = new SFXEntry{ subdivision, textPath + "PeaShooter_Loop.wav" };
	m_pSFXEntries[SFXType::Roundabout_Spawn] = new SFXEntry{ subdivision, textPath + "Roundabout_Spawn.wav" };
	m_pSFXEntries[SFXType::SpreadShot_Loop] = new SFXEntry{ subdivision, textPath + "SpreadShot_Loop.wav" };

	SetVolume();
}

void ParticleSoundeffectManager::InitializeParticlesTutorial()
{
	std::string textPath{ "./resources/images/levels/tutorial/effects/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::Tutorial };

	m_ParticleEntries[ParticleType::Explosion] = new ParticleEntry{ subdivision, textPath + "Explosion.png", 6, 2, 12 };
}

void ParticleSoundeffectManager::InitializeParticlesBotanicPanic()
{
	std::string textPath{ "./resources/images/levels/botanicpanic/effects/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::BotanicPanic };

	m_ParticleEntries[ParticleType::Boss_Explosion] = new ParticleEntry{ subdivision, textPath + "Boss_Explosion.png", 3, 3, 9};
	m_ParticleEntries[ParticleType::Carrot_Torpedo_Death] = new ParticleEntry{subdivision, textPath + "Carrot_Torpedo_Death.png", 5, 2, 9 };
	m_ParticleEntries[ParticleType::DirtBall_Death] = new ParticleEntry{ subdivision, textPath + "DirtBall_Death.png", 5, 1, 5 };
	m_ParticleEntries[ParticleType::Horace_Radiche_Leaves] = new ParticleEntry{ subdivision, textPath + "Horace_Radiche_Leaves.png", 7, 2, 13 };
	m_ParticleEntries[ParticleType::Horace_Radiche_Smoke] = new ParticleEntry{ subdivision, textPath + "Horace_Radiche_Smoke.png", 5, 3, 14 };
	m_ParticleEntries[ParticleType::Potato_Spit] = new ParticleEntry{ subdivision, textPath + "Potato_Spit.png", 3, 3, 9 };
	m_ParticleEntries[ParticleType::Tear_Splash] = new ParticleEntry{ subdivision, textPath + "Tear_Splash.png", 3, 2, 6};
	m_ParticleEntries[ParticleType::Tear_Splash_Pink] = new ParticleEntry{ subdivision, textPath + "Tear_Splash_Pink.png", 3, 2, 6 };
	m_ParticleEntries[ParticleType::Worm_Death] = new ParticleEntry{ subdivision, textPath + "Worm_Death.png", 4, 1, 4 };
}

void ParticleSoundeffectManager::InitializeSFXBotanicPanic()
{
	std::string textPath{ "./Resources/sounds/Effects/botanicpanic/" };

	Enums::EffectSubDivision subdivision{ Enums::EffectSubDivision::BotanicPanic };

	m_pSFXEntries[SFXType::Announcer_Intro] = new SFXEntry{ subdivision, textPath + "Announcer_Intro.wav" };
	m_pSFXEntries[SFXType::Announcer_Knockout] = new SFXEntry{ subdivision, textPath + "Announcer_Knockout.wav" };
	m_pSFXEntries[SFXType::Boss_Death] = new SFXEntry{ subdivision, textPath + "Boss_Death.wav" };
	m_pSFXEntries[SFXType::Chauncey_Chantenay_Attack] = new SFXEntry{ subdivision, textPath + "Chauncey_Chantenay_Attack.wav" };
	m_pSFXEntries[SFXType::Chauncey_Chantenay_Beam] = new SFXEntry{ subdivision, textPath + "Chauncey_Chantenay_Beam.wav" };
	m_pSFXEntries[SFXType::Chauncey_Chantenay_Carrot_Death] = new SFXEntry{ subdivision, textPath + "Chauncey_Chantenay_Carrot_Death.wav" };
	m_pSFXEntries[SFXType::Chauncey_Chantenay_Rise] = new SFXEntry{ subdivision, textPath + "Chauncey_Chantenay_Rise.wav" };
	m_pSFXEntries[SFXType::Horace_Radiche_Death] = new SFXEntry{ subdivision, textPath + "Horace_Radiche_Death.wav" };
	m_pSFXEntries[SFXType::Horace_Radiche_Explode] = new SFXEntry{ subdivision, textPath + "Horace_Radiche_Explode.wav" };
	m_pSFXEntries[SFXType::Horace_Radiche_Intro] = new SFXEntry{ subdivision, textPath + "Horace_Radiche_Intro.wav" };
	m_pSFXEntries[SFXType::Ollie_Bulb_Crying] = new SFXEntry{ subdivision, textPath + "Ollie_Bulb_Crying.wav" };
	m_pSFXEntries[SFXType::Ollie_Bulb_Happy] = new SFXEntry{ subdivision, textPath + "Ollie_Bulb_Happy.wav" };
	m_pSFXEntries[SFXType::Ollie_Bulb_Rising] = new SFXEntry{ subdivision, textPath + "Ollie_Bulb_Rising.wav" };
	m_pSFXEntries[SFXType::Ollie_Bulb_Tear_Death] = new SFXEntry{ subdivision, textPath + "Ollie_Bulb_Tear_Death.wav" };
	m_pSFXEntries[SFXType::Sal_Spudder_Object_Death] = new SFXEntry{ subdivision, textPath + "Sal_Spudder_Object_Death.wav" };
	m_pSFXEntries[SFXType::Sal_Spudder_Rising] = new SFXEntry{ subdivision, textPath + "Sal_Spudder_Rising.wav" };
	m_pSFXEntries[SFXType::Sal_Spudder_Spit] = new SFXEntry{ subdivision, textPath + "Sal_Spudder_Spit.wav" };

	SetVolume();
}
