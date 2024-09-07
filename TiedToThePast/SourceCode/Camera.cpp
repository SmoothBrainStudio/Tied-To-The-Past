#include "Camera.hpp"
#include "Player.hpp"
//Copyright 2024 Property of Olivier Maurin.All rights reserved.

Camera::Camera(Player& _player)
{
	Init(_player);
}

void Camera::Init(Player& _player)
{
	view.setSize(Screen::Get().GetWidth(), Screen::Get().GetHeight());
	view.setCenter(Screen::Get().PosX(_player.getPosition().x), Screen::Get().PosY(_player.getPosition().y - _player.GetGlobalAABB().height / 2.f));
}

void Camera::Update(Player& _player)
{
	if (view.getSize().x != Screen::Get().GetWidth() || view.getSize().y != Screen::Get().GetHeight())
	{
		view.setSize(Screen::Get().GetWidth(), Screen::Get().GetHeight());
		SetCenterWithBorders(sf::Vector2f(_player.getPosition().x, _player.getPosition().y - _player.GetGlobalAABB().height / 2.f));
	}

	IsCollidingWithMapBorders(_player);

	Move(_player);
	MoveToDesiredPosition();
	MoveToCenteredEntitiesPos();
	CinematicMove();
	CameraShake();
	Zoom();
}

void Camera::IsCollidingWithMapBorders(Player& _player)
{
	Screen& screen = Screen::Get();
	sf::Vector2f playerPos = screen.Pos(_player.getPosition());
	float playerAABBHeight = screen.PosY(_player.GetGlobalAABB().height);
	const float& screenW = screen.GetWidth();
	const float& screenH = screen.GetHeight();
	sf::Vector2f offset = screen.Pos(border.offsetX, border.offsetY);
	sf::Vector2f max = screen.Pos(border.maxX, border.maxY);

	if (!((playerPos.x < (screenW / 2.f + offset.x)) ||
		(playerPos.x > (max.x - screenW / 2.f)) ||
		((playerPos.y - playerAABBHeight / 2.f) < (screenH / 2.f + offset.y)) ||
		((playerPos.y - playerAABBHeight / 2.f) > (max.y - screenH / 2.f))))
	{
		locked = false;
	}
	else
	{
		locked = true;
	}
}

void Camera::CameraShake(void)
{
	if (shakeTimer > 0.f && shakeEnable)
	{
		Screen& screen = Screen::Get();
		// Calculer la position aléatoire de l'effet de shake
		float randX = (rand() % 100) / 50.0f - 1.0f;
		float randY = (rand() % 100) / 50.0f - 1.0f;

		// Appliquer l'effet de shake en fonction de l'intensité et de la durée
		shakeOffsetX = screen.PosX(randX * shakeIntensity * shakeTimer / shakeDuration);
		shakeOffsetY = screen.PosY(randY * shakeIntensity * shakeTimer / shakeDuration);

		// Mettre à jour la position de la vue de la caméra avec l'effet de shake
		view.setCenter(view.getCenter().x + shakeOffsetX, view.getCenter().y + shakeOffsetY);

		// Réduire le timer de l'effet de shake
		shakeTimer -= GameClock::Get().GetDeltaTime();
	}
	else
	{
		shakeEnable = false;
	}
}

void Camera::Move(Player& _player)
{
	const float& dt = GameClock::Get().GetDeltaTime();
	Screen& screen = Screen::Get();
	sf::Vector2f playerPos = screen.Pos(_player.getPosition());
	float playerOffsetOriginY = screen.PosY(_player.GetGlobalAABB().height / 2.f);

	if (!locked && cameraState == CameraState::FOCUS_PLAYER)
	{
		sf::Vector2f playerCenterPos = sf::Vector2f(playerPos.x, playerPos.y - playerOffsetOriginY);

		sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), playerCenterPos);
		float distance = Maths::GetVectorLength(view.getCenter(), playerCenterPos);

		sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
		view.setCenter(result);

	}
	else if (locked && cameraState == CameraState::FOCUS_PLAYER)
	{
		const float& screenW = screen.GetWidth();
		const float& screenH = screen.GetHeight();
		const sf::Vector2f offset = screen.Pos(border.offsetX, border.offsetY);
		const sf::Vector2f max = screen.Pos(border.maxX, border.maxY);

		const bool collideLeft = playerPos.x < (screenW / 2.f + offset.x);
		const bool collideRight = playerPos.x > (max.x - screenW / 2.f);
		const bool collideUp = (playerPos.y - playerOffsetOriginY) < (screenH / 2.f + offset.y);
		const bool collideDown = (playerPos.y - playerOffsetOriginY) > (max.y - screenH / 2.f);

		if (collideLeft && !collideRight && !collideUp && !collideDown)
		{
			sf::Vector2f movePos = { screenW / 2.f + offset.x , playerPos.y - playerOffsetOriginY };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideRight && !collideLeft && !collideUp && !collideDown)
		{
			sf::Vector2f movePos = { max.x - screenW / 2.f , playerPos.y - playerOffsetOriginY };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideUp && !collideRight && !collideLeft && !collideDown)
		{
			sf::Vector2f movePos = { playerPos.x , screenH / 2.f + offset.y };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideDown && !collideRight && !collideUp && !collideLeft)
		{
			sf::Vector2f movePos = { playerPos.x , max.y - screenH / 2.f };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideLeft && collideUp)
		{
			sf::Vector2f movePos = { screenW / 2.f + offset.x , screenH / 2.f + offset.y };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideRight && collideDown)
		{
			sf::Vector2f movePos = { max.x - screenW / 2.f , max.y - screenH / 2.f };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideLeft && collideDown)
		{
			sf::Vector2f movePos = { screenW / 2.f + offset.x , max.y - screenH / 2.f };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED* screen.GetAspectRatio();
			view.setCenter(result);
		}
		else if (collideRight && collideUp)
		{
			sf::Vector2f movePos = { max.x - screenW / 2.f , screenH / 2.f + offset.y };

			sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), movePos);
			float distance = Maths::GetVectorLength(view.getCenter(), movePos);

			sf::Vector2f result = view.getCenter() + vectorBetween * dt * CAMERA_SPEED * screen.GetAspectRatio();
			view.setCenter(result);
		}
	}
}

void Camera::MoveToDesiredPosition(void)
{
	if (cameraState == CameraState::MOVING_TO_ENTITY)
	{
		sf::Vector2f vectorBetween = Maths::CreateVector(view.getCenter(), desiredPosition);
		float distance = Maths::GetVectorLength(view.getCenter(), desiredPosition);

		sf::Vector2f result = view.getCenter() + vectorBetween * GameClock::Get().GetDeltaTime() * CAMERA_SPEED * Screen::Get().GetAspectRatio();
		view.setCenter(result);

		if (Maths::GetVectorLength(desiredPosition, view.getCenter()) < (Screen::Get().GetAspectRatio() * 10.f))
		{
			cameraState = CameraState::FOCUS_PLAYER;

			if (isInCinematic)
			{
				cinematicTimer = 0.f;
				isInCinematic = false;
			}
		}
	}
}

void Camera::CinematicMove(void)
{
	if (isInCinematic && !shakeEnable)
	{
		cinematicTimer += GameClock::Get().GetDeltaTime();
		if (cinematicTimer >= CINEMATIC_WAITING_TIME && cameraState == CameraState::FOCUS_PLAYER)
		{
			cameraState = CameraState::MOVING_TO_ENTITY;
		}
	}
}

void Camera::MoveToCenteredEntitiesPos(void)
{
	if (cameraState == CameraState::CENTERING_2_ENTITIES)
	{
		sf::Vector2f movement = center2entitiesPosition - view.getCenter();
		view.move(movement * CAMERA_SPEED * Screen::Get().GetAspectRatio() * GameClock::Get().GetDeltaTime());
	}
}

void Camera::Zoom(void)
{
	if (zoomEnable)
	{
		zoomTimer += GameClock::Get().GetDeltaTime();
		float factor = std::min((zoomTimer / zoomDuration) * zoomIntensity, zoomIntensity);

		if (zoomType == ZoomType::IN)
		{
			view.zoom(1.f - factor);
		}
		else
		{
			view.zoom(1.f + factor);
		}

		if (zoomTimer >= zoomDuration)
		{
			zoomEnable = false;
		}

	}
}

const sf::View& Camera::GetView(void) const
{
	return view;
}

void Camera::SetCameraShake(const float& _duration, const float& _shakeIntensity)
{
	shakeEnable = true;
	shakeDuration = _duration;
	shakeIntensity = _shakeIntensity;

	if (shakeEnable)
	{
		shakeTimer = shakeDuration;
	}
	else
	{
		shakeTimer = 0.f;
	}

	shakeOffsetX = 0.f;
	shakeOffsetY = 0.f;
}

void Camera::SetZoom(ZoomType _type, const float& _duration, const float& _intensity)
{
	zoomEnable = true;
	zoomDuration = _duration;
	zoomIntensity = _intensity;
	zoomType = _type;
	zoomTimer = 0.f;
}

void Camera::SetDesiredPos(const sf::Vector2f& _pos)
{
	desiredPosition = Screen::Get().Pos(_pos);
	cameraState = CameraState::MOVING_TO_ENTITY;
}

void Camera::SetCinematicMove(const sf::Vector2f& _pos, const float& _duration, const float& _shakeIntensity)
{
	desiredPosition = _pos;
	SetCameraShake(_duration, _shakeIntensity);
	isInCinematic = true;
}

void Camera::SetCenterBetween2Entities(Entity& _entity1, Entity& _entity2)
{
	cameraState = CameraState::CENTERING_2_ENTITIES;
	sf::Vector2f distance = { _entity2.getPosition().x - _entity1.getPosition().x, _entity2.getPosition().y - _entity1.getPosition().y };
	center2entitiesPosition = Screen::Get().Pos(sf::Vector2f(_entity1.getPosition().x + distance.x / 2.f, _entity1.getPosition().y + distance.y / 2.f));
}

void Camera::SetCameraState(const CameraState& _state)
{
	cameraState = _state;
}

void Camera::SetCenterWithBorders(const sf::Vector2f& _pos)
{
	Screen& screen = Screen::Get();
	const float& screenW = screen.GetWidth();
	const float& screenH = screen.GetHeight();
	sf::Vector2f offset = screen.Pos(border.offsetX, border.offsetY);
	sf::Vector2f max = screen.Pos(border.maxX, border.maxY);

	sf::Vector2f movePos = screen.Pos(_pos);

	const bool collideLeft = movePos.x < (screenW / 2.f + offset.x);
	const bool collideRight = movePos.x > (max.x - screenW / 2.f);
	const bool collideUp = movePos.y < (screenH / 2.f + offset.y);
	const bool collideDown = movePos.y > (max.y - screenH / 2.f);

	if (collideLeft && !collideRight && !collideUp && !collideDown)
	{
		movePos = { screenW / 2.f + offset.x , movePos.y };
	}
	else if (collideRight && !collideLeft && !collideUp && !collideDown)
	{
		movePos = { max.x - screenW / 2.f , movePos.y };
	}
	else if (collideUp && !collideRight && !collideLeft && !collideDown)
	{
		movePos = { movePos.x , screenH / 2.f + offset.y };
	}
	else if (collideDown && !collideRight && !collideUp && !collideLeft)
	{
		movePos = { movePos.x , max.y - screenH / 2.f };
	}
	else if (collideLeft && collideUp)
	{
		movePos = { screenW / 2.f + offset.x , screenH / 2.f + offset.y };
	}
	else if (collideRight && collideDown)
	{
		movePos = { max.x - screenW / 2.f , max.y - screenH / 2.f };
	}
	else if (collideLeft && collideDown)
	{
		movePos = { screenW / 2.f + offset.x , max.y - screenH / 2.f };
	}
	else if (collideRight && collideUp)
	{
		movePos = { max.x - screenW / 2.f , screenH / 2.f + offset.y };
	}

	view.setCenter(movePos);
}

void Camera::AddOffset(const sf::Vector2f& _offset)
{
	view.move(Screen::Get().Pos(_offset));
}

std::ostream& operator << (std::ostream& _buffer, Camera& _camera)
{
	const sf::Vector2f& pos = _camera.view.getCenter();
	_buffer << pos.x << ' ';
	_buffer << pos.y;
	return _buffer;
}

std::istream& operator >> (std::istream& _buffer, Camera& _camera)
{
	sf::Vector2f pos;
	_buffer >> pos.x;
	_buffer >> pos.y;
	_camera.view.setCenter(pos);
	_camera.desiredPosition = pos;
	return _buffer;
}
