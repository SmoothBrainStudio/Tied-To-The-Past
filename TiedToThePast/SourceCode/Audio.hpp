#ifndef AUDIO__HPP
#define AUDIO__HPP

#include"Definitions.hpp"
#include"Screen.hpp"

constexpr float SOUND_DISTANCE_FACTOR = 250.f;

enum PlaySoundType
{
	RANDOM,
	CRESCENDO,
	DECRESCENDO
};

class Audio
{
public:
	~Audio();
	Audio(const Audio&) = delete;
	Audio(Audio&&) = delete;
	Audio& operator=(const Audio&) = delete;
	Audio& operator=(Audio&&) = delete;

	static Audio& Get(void);
	void Init(void);

	const bool AddBuffer(const std::string& _fileName, const std::string& _key);
	const bool AddSoundWithBuffer(const std::string& _keyBuffer, const std::string& _keySound, const float& _volume = 100.f);
	const bool AddSoundAndBuffer(const std::string& _fileName, const std::string& _keyBuffer, const std::string& _keySound, const float& _volume = 100.f);
	const bool AddSoundAndBuffer(const std::string& _fileName, const std::string& _key, const float& _volume = 100.f);
	const bool AddSoundWithMultipleBuffers(std::vector<std::string>& _bufferKeyVect, const std::string& _keySound, const PlaySoundType& _playType, const float& _stepTime, const float& _volume = 100.f);
	const bool AddMusic(const std::string& _fileName, const std::string& _key);

	void DeleteSound(const std::string& _key, std::string str = __builtin_FUNCTION());
	void DeleteBuffer(const std::string& _key, std::string str = __builtin_FUNCTION());
	void DeleteMusic(const std::string& _key, std::string str = __builtin_FUNCTION());
	void DeleteSoundAndBuffer(const std::string& _key, std::string str = __builtin_FUNCTION());

	void PlaySound(const std::string& _key, const float& _volume = -1.f);
	void PlaySoundNoSecurity(const std::string& _key, const float& _volume = -1.f);
	void PlaySoundWithType(const std::string& _key, const float& _dt, const float& _volume = -1.f);
	void StopSound(const std::string& _key);
	void StopAllSounds(void);
	void PauseAllSounds(void);
	void UnPauseAllSounds(void);

	void PlayMusic(const std::string& _key, const bool& _setLoop, const float& _volume = 100.f);
	void StopMusic(const std::string& _key);
	void StopAllMusics(void);
	void PauseMusic(const std::string& _key);
	void PauseAllMusics(void);
	void UnPauseMusic(const std::string& _key);
	void UnPauseAllMusics(void);

	void ChangeBuffer(const std::string& _keySound, const std::string& _newKeyBuffer);


	void SetSoundVolume(const std::string& _key, const float& _volume = 100.f);

	//marche seulement pour les sons mono
	void SetSoundPosition(const std::string& _key, const sf::Vector2f& _pos, const float& _z = 0.f);

	//marche seulement pour les sons mono
	void SetSoundPosition(const std::string& _key, const float& _x, const float& _y, const float& _z = 0.f);

	//mettre le bool à false pour jouer un son relatif à la position du listner. _minDist donne la distance à jusqu'à laquelle
	//le son est joué à son volume max et plus l'atténuation est grand, plus le son diminue vite en fonction de la distance
	void SetRelativeToListner(const std::string& _key, const bool& _relative, const float& _minDist, const float& _Attenuation);

	void SetSoundPitch(const std::string& _key, const float& _pitch = 1.f);

	void SetMusicVolume(const std::string& _key, const float& _volume = 100.f);

	//marche seulement pour les sons mono
	void SetMusicPosition(const std::string& _key, const sf::Vector2f& _pos, const float& _z = 0.f);

	//marche seulement pour les sons mono
	void SetMusicPosition(const std::string& _key, const float& _x, const float& _y, const float& _z = 0.f);

	void SetMusicPitch(const std::string& _key, const float& _pitch);

	const sf::SoundBuffer& GetSoundBuffer(const std::string& _key);
	const sf::Sound::Status GetSoundStatus(const std::string& _key);

	const float GetSoundEffectiveVolume(const std::string& _key);
	const float GetSoundVolume(const std::string& _key);

	const sf::Sound& GetSound(const std::string& _key);
	const sf::SoundBuffer& GetBufferOnSound(const std::string& _key);
	const unsigned int& GetSoundNbTimesPlayed(const std::string& _key);

	const sf::Music::Status GetMusicStatus(const std::string& _key);
	const float GetEffectiveMusicVolume(const std::string& _key);
	const float GetMusicVolume(const std::string& _key);

	const float& GetSoundMultiplier(void);
	const float& GetMusicMultiplier(void);
	void SetSoundMultiplier(const float& _soundMultiplier);
	void SetMusicMultiplier(const float& _musicMultiplier);
	void ModifySoundMultiplier(const float& _modifier);
	void ModifyMusicMultiplier(const float& _modifier);

	const bool IsFileAlreadyInMap(const std::string& _filePath);

	static const std::string GenerateRdmName(const int& _lenght = 10);

private:
	Audio() = default;
	void SetAllMusicsVolume(void);
	void SetAllSoundsVolume(void);

	void PlaySoundRandomType(const std::string& _key, const float& _dt);
	void PlaySoundCrescendoType(const std::string& _key, const float& _dt);
	void PlaySoundDecrescendoType(const std::string& _key, const float& _dt);

	void SavePreference(const std::string& _name, const float& _value);

	struct SoundBufferContainer
	{
		~SoundBufferContainer() = default;
		std::unique_ptr<sf::SoundBuffer> buffer;
		std::string fileName;
	};

	struct Music
	{
		sf::Music music;
		std::string fileName;
		float volume = 100.f;
	};

	struct Sound
	{
		~Sound() = default;
		sf::Sound sound;
		std::vector<std::string> keyBufferVect;
		float timeStep = 0.f;
		float timer = 0.f;
		short int bufferID = -1;
		std::function<void(const std::string&, const float&)> playSoundType;
		unsigned int nbTimesPlayed = 0;
		float volume = 100.f;
	};

	std::unordered_map<std::string, std::unique_ptr<SoundBufferContainer>> soundBufferMap;
	std::unordered_map<std::string, std::unique_ptr<Sound>> soundMap;
	std::unordered_map<std::string, std::unique_ptr<Music>> musicMap;

	float soundMultiplier = 0.5f;
	float musicMultiplier = 0.5f;
};
#endif // ! AUDIO__HPP