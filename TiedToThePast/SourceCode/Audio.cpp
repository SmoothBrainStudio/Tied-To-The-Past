#include "Definitions.hpp"
#include "Audio.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

Audio& Audio::Get(void)
{
	static Audio myInstance;
	return myInstance;
}

void Audio::Init(void)
{
	Json::Value preferences = JsonManager::LoadFromFile(PREFERENCES_PATH);

	SetMusicMultiplier(preferences["audio"]["music"].asFloat() / 100.f);
	SetSoundMultiplier(preferences["audio"]["sound"].asFloat() / 100.f);
}

Audio::~Audio()
{
}

const bool Audio::AddBuffer(const std::string& _fileName, const std::string& _key)
{
	const auto it = std::find_if(soundBufferMap.begin(), soundBufferMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<SoundBufferContainer>>& _buffer)
		{
			return _fileName == _buffer.second->fileName;
		});

	if (it != soundBufferMap.end())
	{
		Debug.warning("Buffer \"", _fileName, "\" is already in map.");
		return false;
	}

	std::unique_ptr<SoundBufferContainer> temp = std::make_unique<SoundBufferContainer>();
	temp->buffer = std::make_unique<sf::SoundBuffer>();
	if (temp->buffer->loadFromFile(_fileName))
	{
		temp->fileName = _fileName;
		soundBufferMap[_key] = std::move(temp);
		Debug.system("Buffer \"", _fileName, "\" is register with the key \"", _key, "\".");
		return true;
	}
	else
	{
		temp->buffer.release();
		temp.release();
		Debug.warning("Buffer \"", _fileName, "\" don't exist in assets.");
		return false;
	}
}

const bool Audio::AddSoundWithBuffer(const std::string& _keyBuffer, const std::string& _keySound, const float& _volume)
{
	if (soundMap.find(_keySound) != soundMap.end())
	{
		Debug.warning("Sound \"", _keySound, "\" is already in map.");
		return false;
	}

	auto temp = std::make_unique<Sound>();
	temp->sound.setBuffer(*(soundBufferMap[_keyBuffer]->buffer));
	temp->volume = _volume;
	temp->sound.setVolume(_volume * soundMultiplier);
	soundMap[_keySound] = std::move(temp);

	Debug.system("Sound \"", _keySound, "\" is register with buffer \"", _keyBuffer, "\".");
	return true;
}

const bool Audio::AddSoundAndBuffer(const std::string& _fileName, const std::string& _keyBuffer, const std::string& _keySound, const float& _volume)
{
	const auto it = std::find_if(soundBufferMap.begin(), soundBufferMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<SoundBufferContainer>>& _buffer)
		{
			return _fileName == _buffer.second->fileName;
		});

	if (soundMap.find(_keySound) != soundMap.end())
	{
		Debug.warning("Sound \"", _fileName, "\" is already in map.");
		return false;
	}

	if (it != soundBufferMap.end())
	{
		Debug.warning("Buffer \"", _keyBuffer, "\" is already in map.");
		return false;
	}

	std::unique_ptr<SoundBufferContainer> tempBuffer = std::make_unique<SoundBufferContainer>();

	tempBuffer->buffer = std::make_unique<sf::SoundBuffer>();
	if (tempBuffer->buffer->loadFromFile(_fileName))
	{
		tempBuffer->fileName = _fileName;
		soundBufferMap[_keyBuffer] = std::move(tempBuffer);

		auto tempSound = std::make_unique<Sound>();
		tempSound->sound.setBuffer(*(soundBufferMap[_keyBuffer]->buffer));
		tempSound->volume = _volume;
		tempSound->sound.setVolume(_volume * soundMultiplier);
		soundMap[_keySound] = std::move(tempSound);

		Debug.system("Buffer \"", _fileName, "\" is register with the key \"", _keySound, "\" in the buffer \"", _keyBuffer, "\".");
		return true;
	}
	else
	{
		tempBuffer->buffer.release();
		tempBuffer.release();

		Debug.warning("Buffer \"", _keyBuffer, "\" don't exist in assets.");
		return false;
	}
}

const bool Audio::AddSoundAndBuffer(const std::string& _fileName, const std::string& _key, const float& _volume)
{
	const auto it = std::find_if(soundBufferMap.begin(), soundBufferMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<SoundBufferContainer>>& _buffer)
		{
			return _fileName == _buffer.second->fileName;
		});

	if (soundMap.find(_key) != soundMap.end())
	{
		Debug.warning("Sound \"", _key, "\" is already in map.");
		return false;
	}

	if (it != soundBufferMap.end())
	{
		Debug.warning("Buffer \"", _fileName, "\" is already in map.");
		return false;
	}


	std::unique_ptr<SoundBufferContainer> tempBuffer = std::make_unique<SoundBufferContainer>();
	tempBuffer->buffer = std::make_unique<sf::SoundBuffer>();

	if (tempBuffer->buffer->loadFromFile(_fileName))
	{
		tempBuffer->fileName = _fileName;

		soundBufferMap[_key] = std::move(tempBuffer);

		auto tempSound = std::make_unique<Sound>();
		tempSound->sound.setBuffer(*(soundBufferMap[_key]->buffer));
		tempSound->volume = _volume;
		tempSound->sound.setVolume(_volume * soundMultiplier);
		soundMap[_key] = std::move(tempSound);

		Debug.system("Buffer \"", _fileName, "\" is register with the key \"", _key, "\".");
		return true;
	}
	else
	{
		tempBuffer->buffer.release();
		tempBuffer.release();

		Debug.warning("Buffer \"", _fileName, "\" don't exist in assets.");
		return false;
	}
}

const bool Audio::AddSoundWithMultipleBuffers(std::vector<std::string>& _keyBufferVect, const std::string& _keySound, const PlaySoundType& _playType, const float& _stepTime, const float& _volume)
{
	if (soundMap.find(_keySound) != soundMap.end())
	{
		Debug.warning("Sound \"", _keySound, "\" is already in map.");
		return false;
	}

	auto temp = std::make_unique<Sound>();
	temp->sound.setBuffer(*(soundBufferMap[_keyBufferVect[0]]->buffer));
	temp->volume = _volume;
	temp->sound.setVolume(_volume * soundMultiplier);
	temp->sound.setPosition(Screen::Get().GetWidth() / 2.f, Screen::Get().GetHeight() / 2.f, 0.f);
	temp->keyBufferVect = _keyBufferVect;
	temp->timeStep = _stepTime;
	temp->bufferID = 0;

	switch (_playType)
	{
	case RANDOM:
		temp->playSoundType = std::bind(&Audio::PlaySoundRandomType, this, std::placeholders::_1, std::placeholders::_2);
		break;
	case CRESCENDO:
		temp->playSoundType = std::bind(&Audio::PlaySoundCrescendoType, this, std::placeholders::_1, std::placeholders::_2);
		break;
	case DECRESCENDO:
		temp->playSoundType = std::bind(&Audio::PlaySoundDecrescendoType, this, std::placeholders::_1, std::placeholders::_2);
		break;
	default:
		break;
	}
	soundMap[_keySound] = std::move(temp);

	Debug.system("Sound \"", _keySound, "\" is register in map.");
	return true;
}

const bool Audio::AddMusic(const std::string& _fileName, const std::string& _key)
{
	const auto it = std::find_if(musicMap.begin(), musicMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<Music>>& _music)
		{
			return _fileName == _music.second->fileName;
		});

	if (it != musicMap.end())
	{
		Debug.warning("Music \"", _fileName, "\" is already in map.");
		return false;
	}

	auto temp = std::make_unique<Music>();
	if (temp->music.openFromFile(_fileName))
	{
		temp->fileName = _fileName;
		temp->music.setPosition(Screen::Get().GetWidth() / 2.f, Screen::Get().GetHeight() / 2.f, 0.f);
		musicMap[_key] = std::move(temp);

		Debug.system("Music \"", _fileName, "\" is register with the key \"", _key, "\".");
		return true;
	}
	else
	{
		Debug.warning("Music \"", _fileName, "\" don't exist in assets.");
		return false;
	}
}

void Audio::DeleteSound(const std::string& _key, std::string str)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		Debug.system("Sound \"", _key, "\" was delete.");
		soundMap.erase(_key);
	}
	else
	{
		Debug.warning("Key, \"", _key, "\" don't exist in map, Function \"DeleteSound()\" from : ", str);
	}
}

void Audio::DeleteBuffer(const std::string& _key, std::string str)
{
	if (soundBufferMap.find(_key) != soundBufferMap.end())
	{
		Debug.system("Buffer \"", _key, "\" was delete.");
		soundBufferMap.erase(_key);
	}
	else
	{
		Debug.warning("Key, \"", _key, "\" don't exist in map, Function \"DeleteBuffer()\" from : ", str);
	}
}

void Audio::DeleteMusic(const std::string& _key, std::string str)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		Debug.system("Music \"", _key, "\" was delete.");
		musicMap.erase(_key);
	}
	else
	{
		Debug.warning("Key, \"", _key, "\" don't exist in map, Function \"DeleteMusic()\" from : ", str);
	}
}

void Audio::DeleteSoundAndBuffer(const std::string& _key, std::string str)
{
	if (soundBufferMap.find(_key) != soundBufferMap.end())
	{
		Debug.system("Buffer \"", _key, "\" was delete.");
		soundBufferMap.erase(_key);
	}
	else
	{
		Debug.warning("Key, \"", _key, "\" don't exist in map, Function \"DeleteBuffer()\" from : ", str);
	}

	if (soundMap.find(_key) != soundMap.end())
	{
		Debug.system("Sound \"", _key, "\" was delete.");
		soundMap.erase(_key);
	}
	else
	{
		Debug.warning("Key, \"", _key, "\" don't exist in map, Function \"DeleteSound()\" from : ", str);
	}
}

void Audio::PlaySound(const std::string& _key, const float& _volume)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		if (soundMap[_key]->sound.getStatus() == sf::Sound::Stopped
			|| soundMap[_key]->sound.getStatus() == sf::Sound::Paused)
		{
			if (_volume == -1.f)
			{
				soundMap[_key]->sound.setVolume(soundMap[_key]->volume * soundMultiplier);
			}
			else
			{
				soundMap[_key]->volume = _volume;
				soundMap[_key]->sound.setVolume(_volume * soundMultiplier);
			}

			soundMap[_key]->sound.play();
			soundMap[_key]->nbTimesPlayed++;
		}
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::PlaySoundNoSecurity(const std::string& _key, const float& _volume)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		if (_volume == -1.f)
		{
			soundMap[_key]->sound.setVolume(soundMap[_key]->volume * soundMultiplier);
		}
		else
		{
			soundMap[_key]->volume = _volume;
			soundMap[_key]->sound.setVolume(_volume * soundMultiplier);
		}

		soundMap[_key]->sound.play();
		soundMap[_key]->nbTimesPlayed++;
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::PlaySoundWithType(const std::string& _key, const float& _dt, const float& _volume)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		soundMap[_key]->playSoundType(_key, _dt);
		soundMap[_key]->nbTimesPlayed++;
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::StopSound(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		if (soundMap[_key]->sound.getStatus() == sf::Sound::Playing)
		{
			soundMap[_key]->sound.stop();
		}
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::StopAllSounds(void)
{
	for (auto& it : soundMap)
	{
		if (it.second->sound.getStatus() == sf::Sound::Playing)
		{
			it.second->sound.stop();
		}
	}
}

void Audio::PauseAllSounds(void)
{
	for (auto& it : soundMap)
	{
		if (it.second->sound.getStatus() == sf::Sound::Playing)
		{
			it.second->sound.pause();
		}
	}
}

void Audio::UnPauseAllSounds(void)
{
	for (auto& it : soundMap)
	{
		if (it.second->sound.getStatus() == sf::Sound::Paused)
		{
			it.second->sound.play();
		}
	}
}

void Audio::PlayMusic(const std::string& _key, const bool& _setLoop, const float& _volume)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		if (musicMap[_key]->music.getStatus() == sf::Music::Stopped || musicMap[_key]->music.getStatus() == sf::Music::Paused)
		{
			musicMap[_key]->music.setPosition(Screen::Get().GetWidth() / 2.f, Screen::Get().GetHeight() / 2.f, 0.f);
			musicMap[_key]->volume = _volume;
			musicMap[_key]->music.setVolume(_volume * musicMultiplier);
			musicMap[_key]->music.setLoop(_setLoop);
			musicMap[_key]->music.play();
		}
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::StopMusic(const std::string& _key)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		if (musicMap[_key]->music.getStatus() == sf::Music::Playing)
		{
			musicMap[_key]->music.stop();
		}
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::StopAllMusics(void)
{
	for (auto& it : musicMap)
	{
		if (it.second->music.getStatus() == sf::Music::Playing)
		{
			it.second->music.stop();
		}
	}
}

void Audio::PauseMusic(const std::string& _key)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		if (musicMap[_key]->music.getStatus() == sf::Music::Playing)
		{
			musicMap[_key]->music.pause();
		}
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::PauseAllMusics(void)
{
	for (auto& it : musicMap)
	{
		if (it.second->music.getStatus() == sf::Music::Playing)
		{
			it.second->music.pause();
		}
	}
}

void Audio::UnPauseMusic(const std::string& _key)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		if (musicMap[_key]->music.getStatus() == sf::Music::Paused)
		{
			musicMap[_key]->music.play();
		}
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::UnPauseAllMusics(void)
{
	for (auto& it : musicMap)
	{
		if (it.second->music.getStatus() == sf::Music::Paused)
		{
			it.second->music.play();
		}
	}
}

void Audio::ChangeBuffer(const std::string& _keySound, const std::string& _newKeyBuffer)
{
	if (soundMap.find(_keySound) != soundMap.end() && soundBufferMap.find(_newKeyBuffer) != soundBufferMap.end())
	{
		soundMap[_keySound]->sound.setBuffer(*soundBufferMap[_newKeyBuffer]->buffer);
	}
	else
	{
		Debug.warning("DAVID T'ABUSES");
	}
}

const sf::SoundBuffer& Audio::GetSoundBuffer(const std::string& _key)
{
	if (soundBufferMap.find(_key) != soundBufferMap.end())
	{
		return *soundBufferMap[_key]->buffer;
	}
	else
	{
		Debug.error("Buffer \"", _key, "\" doesn't exist in assets.");
		return *soundBufferMap[_key]->buffer;
	}
}

const sf::Sound::Status Audio::GetSoundStatus(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		return soundMap[_key]->sound.getStatus();
	}
	else
	{
		Debug.error("Sound \"", _key, "\" doesn't exist in assets.");
		return soundMap[_key]->sound.getStatus();
	}
}

const float Audio::GetSoundEffectiveVolume(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		return soundMap[_key]->sound.getVolume();
	}
	else
	{
		Debug.error("Sound \"", _key, "\" doesn't exist in assets.");
		return soundMap[_key]->sound.getVolume();
	}
}

const float Audio::GetSoundVolume(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		return soundMap[_key]->volume;
	}
	else
	{
		Debug.error("Sound \"", _key, "\" doesn't exist in assets.");
		return soundMap[_key]->volume;
	}
}

const sf::Sound& Audio::GetSound(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		return soundMap[_key]->sound;
	}
	else
	{
		Debug.error("Sound \"", _key, "\" doesn't exist in assets.");
		return soundMap[_key]->sound;
	}
}

const sf::SoundBuffer& Audio::GetBufferOnSound(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		return *soundMap[_key]->sound.getBuffer();
	}
	else
	{
		Debug.error("Sound \"", _key, "\" doesn't exist in assets.");
		return *soundMap[_key]->sound.getBuffer();
	}
}

const unsigned int& Audio::GetSoundNbTimesPlayed(const std::string& _key)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		return soundMap[_key]->nbTimesPlayed;
	}
	else
	{
		Debug.error("Sound \"", _key, "\" doesn't exist in assets.");
		return soundMap[_key]->nbTimesPlayed;
	}
}

void Audio::SetSoundVolume(const std::string& _key, const float& _volume)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		soundMap[_key]->volume = _volume;
		soundMap[_key]->sound.setVolume(_volume * soundMultiplier);
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetAllSoundsVolume(void)
{
	for (auto& it : soundMap)
	{
		it.second->sound.setVolume(it.second->volume * soundMultiplier);
	}
}

void Audio::SetSoundPosition(const std::string& _key, const sf::Vector2f& _pos, const float& _z)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		soundMap[_key]->sound.setPosition(Screen::Get().PosX(_pos.x), Screen::Get().PosY(_pos.y), _z);
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetSoundPosition(const std::string& _key, const float& _x, const float& _y, const float& _z)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		soundMap[_key]->sound.setPosition(Screen::Get().PosX(_x), Screen::Get().PosY(_y), _z);
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetRelativeToListner(const std::string& _key, const bool& _relative, const float& _minDist, const float& _Attenuation)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		soundMap[_key]->sound.setRelativeToListener(_relative);
		soundMap[_key]->sound.setMinDistance(_minDist);
		soundMap[_key]->sound.setAttenuation(_Attenuation);
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetSoundPitch(const std::string& _key, const float& _pitch)
{
	if (soundMap.find(_key) != soundMap.end())
	{
		soundMap[_key]->sound.setPitch(_pitch);
	}
	else
	{
		Debug.warning("Sound \"", _key, "\" doesn't exist in assets.");
	}
}

const sf::Music::Status Audio::GetMusicStatus(const std::string& _key)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		return musicMap[_key]->music.getStatus();
	}
	else
	{
		Debug.error("Music \"", _key, "\" doesn't exist in assets.");
		return musicMap[_key]->music.getStatus();
	}
}

const float Audio::GetEffectiveMusicVolume(const std::string& _key)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		return musicMap[_key]->music.getVolume();
	}
	else
	{
		Debug.error("Music \"", _key, "\" doesn't exist in assets.");
		return musicMap[_key]->music.getVolume();
	}
}

const float Audio::GetMusicVolume(const std::string& _key)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		return musicMap[_key]->volume;
	}
	else
	{
		Debug.error("Music \"", _key, "\" doesn't exist in assets.");
		return musicMap[_key]->volume;
	}
}

void Audio::SetMusicVolume(const std::string& _key, const float& _volume)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		musicMap[_key]->volume = _volume;
		musicMap[_key]->music.setVolume(_volume * musicMultiplier);
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetAllMusicsVolume(void)
{
	for (auto& it : musicMap)
	{
		it.second->music.setVolume(it.second->volume * musicMultiplier);
	}
}

void Audio::SetMusicPosition(const std::string& _key, const sf::Vector2f& _pos, const float& _z)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		musicMap[_key]->music.setPosition(Screen::Get().PosX(_pos.x), Screen::Get().PosY(_pos.y), _z);
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetMusicPosition(const std::string& _key, const float& _x, const float& _y, const float& _z)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		musicMap[_key]->music.setPosition(Screen::Get().PosX(_x), Screen::Get().PosY(_y), _z);
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

void Audio::SetMusicPitch(const std::string& _key, const float& _pitch)
{
	if (musicMap.find(_key) != musicMap.end())
	{
		musicMap[_key]->music.setPitch(_pitch);
	}
	else
	{
		Debug.warning("Music \"", _key, "\" doesn't exist in assets.");
	}
}

const float& Audio::GetSoundMultiplier(void)
{
	return soundMultiplier;
}

const float& Audio::GetMusicMultiplier(void)
{
	return musicMultiplier;
}

void Audio::SetSoundMultiplier(const float& _soundMultiplier)
{
	soundMultiplier = _soundMultiplier;
	SetAllSoundsVolume();
	SavePreference("sound", soundMultiplier);
}

void Audio::SetMusicMultiplier(const float& _musicMultiplier)
{
	musicMultiplier = _musicMultiplier;
	SetAllMusicsVolume();
	SavePreference("music", musicMultiplier);
}

void Audio::ModifySoundMultiplier(const float& _modifier)
{
	soundMultiplier += _modifier;

	if (soundMultiplier <= 0.f)
	{
		soundMultiplier = 0.f;
	}
	else if (soundMultiplier >= 1.f)
	{
		soundMultiplier = 1.f;
	}

	SetAllSoundsVolume();
	SavePreference("sound", soundMultiplier);
}

void Audio::ModifyMusicMultiplier(const float& _modifier)
{
	musicMultiplier += _modifier;

	if (musicMultiplier <= 0.f)
	{
		musicMultiplier = 0.f;
	}
	else if (musicMultiplier >= 1.f)
	{
		musicMultiplier = 1.f;
	}

	SetAllMusicsVolume();
	SavePreference("music", musicMultiplier);
}

void Audio::PlaySoundRandomType(const std::string& _key, const float& _dt)
{
	soundMap[_key]->timer += _dt;
	if (soundMap[_key]->timer >= soundMap[_key]->timeStep && !(soundMap[_key]->sound.getStatus() == sf::Sound::Playing))
	{
		int randomIndex = rand() % soundMap[_key]->keyBufferVect.size();
		std::string randomBuffer = soundMap[_key]->keyBufferVect[randomIndex];
		soundMap[_key]->sound.setBuffer(*soundBufferMap[randomBuffer]->buffer);
		soundMap[_key]->timer = 0.f;
	}

	if (!(soundMap[_key]->sound.getStatus() == sf::Sound::Playing))
	{
		soundMap[_key]->sound.play();
	}
}

void Audio::PlaySoundCrescendoType(const std::string& _key, const float& _dt)
{
	soundMap[_key]->timer += _dt;
	if (soundMap[_key]->timer >= soundMap[_key]->timeStep && !(soundMap[_key]->sound.getStatus() == sf::Sound::Playing))
	{
		if (soundMap[_key]->bufferID < soundMap[_key]->keyBufferVect.size())
		{
			++soundMap[_key]->bufferID;
		}
		else
		{
			soundMap[_key]->bufferID = 0;
		}

		std::string bufferStr = soundMap[_key]->keyBufferVect[soundMap[_key]->bufferID];
		soundMap[_key]->sound.setBuffer(*soundBufferMap[bufferStr]->buffer);
		soundMap[_key]->timer = 0.f;
	}

	if (!(soundMap[_key]->sound.getStatus() == sf::Sound::Playing))
	{
		soundMap[_key]->sound.play();
	}
}

void Audio::PlaySoundDecrescendoType(const std::string& _key, const float& _dt)
{
	soundMap[_key]->timer += _dt;
	if (soundMap[_key]->timer >= soundMap[_key]->timeStep && !(soundMap[_key]->sound.getStatus() == sf::Sound::Playing))
	{
		if (soundMap[_key]->bufferID > 0)
		{
			--soundMap[_key]->bufferID;
		}
		else
		{
			soundMap[_key]->bufferID = (short)(soundMap[_key]->keyBufferVect.size());
		}

		std::string bufferStr = soundMap[_key]->keyBufferVect[soundMap[_key]->bufferID];
		soundMap[_key]->sound.setBuffer(*soundBufferMap[bufferStr]->buffer);
		soundMap[_key]->timer = 0.f;
	}

	if (!(soundMap[_key]->sound.getStatus() == sf::Sound::Playing))
	{
		soundMap[_key]->sound.play();
	}
}

void Audio::SavePreference(const std::string& _name, const float& _value)
{
	Json::Value preferences = JsonManager::LoadFromFile(PREFERENCES_PATH);
	preferences["audio"][_name] = _value * 100;

	// Save in file
	std::ofstream file;
	Json::StreamWriterBuilder writer;

	file.open(PREFERENCES_PATH, std::ios::trunc);
	file << Json::writeString(writer, preferences);
	file.close();
}

const bool Audio::IsFileAlreadyInMap(const std::string& _filePath)
{
	const auto it = std::find_if(soundBufferMap.begin(), soundBufferMap.end(),
		[&](const std::pair<const std::string, std::unique_ptr<SoundBufferContainer>>& _buffer)
		{
			return _filePath == _buffer.second->fileName;
		});

	return it != soundBufferMap.end();
}

const std::string Audio::GenerateRdmName(const int& _lenght)
{
	std::string randomName;
	for (int i = 0; i < _lenght; i++)
	{
		randomName += rand() % 26 + 65;
	}

	return randomName;
}