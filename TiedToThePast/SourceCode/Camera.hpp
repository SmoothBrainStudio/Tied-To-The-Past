#ifndef CAMERA__HPP
#define CAMERA__HPP
#include "Definitions.hpp"
#include "Screen.hpp"

static constexpr float SHAKE_INTENSITY = 10.f;
static constexpr float SHAKE_DURATION = 1.f;

static constexpr float ZOOM_INTENSITY = 0.008f;
static constexpr float ZOOM_DURATION = 1.f;

static constexpr float CINEMATIC_WAITING_TIME = 0.5f;
static constexpr float CAMERA_SPEED = 3.5f;

class Player;
class Entity;

class Camera
{
public:
	Camera(Player& _player);
	~Camera() = default;

	enum class ZoomType
	{
		IN,
		OUT
	};

	enum class CameraState
	{
		FOCUS_PLAYER,
		MOVING_TO_ENTITY,
		CENTERING_2_ENTITIES
	};

	void Init(Player& _player);
	void Update(Player& _player);
	
	const sf::View& GetView(void) const;

	void SetCameraShake(const float& _duration = SHAKE_DURATION,const float& _shakeIntensity = SHAKE_INTENSITY);
	void SetZoom(ZoomType _type, const float& _duration = ZOOM_DURATION, const float& _intensity = ZOOM_INTENSITY);
	void SetDesiredPos(const sf::Vector2f& _pos);
	void SetCinematicMove(const sf::Vector2f& _pos, const float& _duration = SHAKE_DURATION, const float& _shakeIntensity = SHAKE_INTENSITY);
	void SetCenterBetween2Entities(Entity& _entity1, Entity& _entity2);
	void SetCameraState(const CameraState& _state);
	void SetCenterWithBorders(const sf::Vector2f& _pos);
	void AddOffset(const sf::Vector2f& _offset);

	void SetBorders(const sf::Vector2f& _offset, const sf::Vector2f& _max)
	{
		border.offsetX = _offset.x;
		border.offsetY = _offset.y;

		border.maxX = _max.x;
		border.maxY = _max.y;
	}
	const CameraBorder& GetBorders(void)
	{
		return border;
	}

	// Operator overloading for saving
	friend std::ostream& operator << (std::ostream& _buffer, Camera& _camera);
	friend std::istream& operator >> (std::istream& _buffer, Camera& _camera);

private:
	Camera() = default;

	void IsCollidingWithMapBorders(Player& _player);

	void CameraShake(void);
	void Zoom(void);

	void Move(Player& _player);
	void MoveToDesiredPosition(void);
	void CinematicMove(void);
	void MoveToCenteredEntitiesPos(void);
	
	sf::View view;
	bool locked = false;
	CameraState cameraState = CameraState::FOCUS_PLAYER;
	CameraBorder border = CameraBorder(0.f, 0.f, WORKING_WIDTH, WORKING_HEIGHT);
	sf::Vector2f desiredPosition = sf::Vector2f(0.f, 0.f);
	float cinematicTimer = 0.f;
	bool isInCinematic = false;
	float timerMoveCam = 0.f;

	//camera shake variables
	float shakeDuration = SHAKE_DURATION;
	float shakeIntensity = SHAKE_INTENSITY;
	float shakeTimer = 0.f;
	float shakeOffsetX = 0.f;
	float shakeOffsetY = 0.f;
	bool shakeEnable = false;

	//camera zoom variables
	float zoomDuration = 0.f;
	float zoomIntensity = 0.f;
	float zoomTimer = 0.f;
	ZoomType zoomType = ZoomType::IN;
	bool zoomEnable = false;

	//centering entities variables
	sf::Vector2f center2entitiesPosition = sf::Vector2f(0.f, 0.f);
};
#endif