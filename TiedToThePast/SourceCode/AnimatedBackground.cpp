#include "Definitions.hpp"
#include "AnimatedBackground.hpp"

void AnimatedBackground::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(layerBackground, states);
	target.draw(moon, states);
	for (int i = 0; i < 5; i++)
	{
		target.draw(clouds[i], states);
	}
	target.draw(lilBoo, states);
	target.draw(bigBoo, states);
	target.draw(layerMansion, states);
	target.draw(owl, states);
	target.draw(layerFront, states);
}

void AnimatedBackground::Init()
{
	// Members
	waitTimeLilboo = Maths::PreciseRand(WAIT_LILBOO_MAX, WAIT_LILBOO_MIN);
	waitTimeOwl = Maths::PreciseRand(WAIT_OWL_MAX, WAIT_OWL_MIN);

	// Elements
	layerBackground.setTexture(texMng["NIGHT_LAYER_1"], true);
	layerMansion.setTexture(texMng["NIGHT_LAYER_2"], true);
	layerFront.setTexture(texMng["NIGHT_LAYER_3"], true);
	layerBackground.setScale(1, 1);
	layerMansion.setScale(1, 1);
	layerFront.setScale(1, 1);

	moon.setTexture(texMng["NIGHT_MOON"], true);
	moon.setPosition(MOON_POSITION);

	owl.LoadFromFile("Contents/Data/Animations/Entities/NPCs/Owl.json");
	owl.setPosition(OWL_POSITION);

	clouds[0].setTexture(texMng["NIGHT_CLOUD_1"], true);
	clouds[1].setTexture(texMng["NIGHT_CLOUD_2"], true);
	clouds[2].setTexture(texMng["NIGHT_CLOUD_3"], true);
	clouds[3].setTexture(texMng["NIGHT_CLOUD_4"], true);
	clouds[4].setTexture(texMng["NIGHT_CLOUD_5"], true);
	clouds[0].setPosition(CLOUD_1_POSITION);
	clouds[1].setPosition(CLOUD_2_POSITION);
	clouds[2].setPosition(CLOUD_3_POSITION);
	clouds[3].setPosition(CLOUD_4_POSITION);
	clouds[4].setPosition(CLOUD_5_POSITION);

	lilBoo.LoadFromFile("Contents/Data/Animations/Entities/NPCs/LilBoo/idle_front.json");
	lilBoo.setPosition(LILBOO_POSITION_1);
	sf::FloatRect rectBoo = lilBoo.GetSprite().getGlobalBounds();
	lilBoo.setOrigin(rectBoo.width / 2.f, rectBoo.height / 2.f);

	bigBoo.setTexture(texMng["INTRO_BIGBOO"], true);
	bigBoo.setPosition(BIGBOO_POSITION_1);
	bigBoo.setScale(0.35f, 0.35f);
	sf::FloatRect rectBigBoo = bigBoo.getGlobalBounds();
	bigBoo.setOrigin(rectBigBoo.width / 2.f, rectBigBoo.height / 2.f);
}

void AnimatedBackground::Update()
{
	const float dt = glMng.GetDeltaTime();
	totalTime += dt;

	// Owl animation update
	owl.Update();

	// Elements update
	moon.setPosition(MOON_POSITION + sf::Vector2f(0.f, sin(totalTime) * 5));
	clouds[0].setPosition(CLOUD_1_POSITION + sf::Vector2f(sin(totalTime) * 6, 0.f));
	clouds[1].setPosition(CLOUD_2_POSITION + sf::Vector2f(sin(totalTime + 2) * 8, 0.f));
	clouds[2].setPosition(CLOUD_3_POSITION + sf::Vector2f(sin(totalTime + 4) * 10, 0.f));
	clouds[3].setPosition(CLOUD_4_POSITION + sf::Vector2f(sin(totalTime + 6) * 12, 0.f));
	clouds[4].setPosition(CLOUD_5_POSITION + sf::Vector2f(sin(totalTime + 8) * 14, 0.f));

	// Entities update
	waitTimeLilboo -= dt;

	if (owl.GetStatus() == AnimatedSprite::Status::STOPPED)
	{
		waitTimeOwl -= dt;
	}

	if (waitTimeOwl <= 0.f)
	{
		owl.Reset();
		owl.Play();
		waitTimeOwl = Maths::PreciseRand(WAIT_OWL_MAX, WAIT_OWL_MIN);
	}

	if (waitTimeLilboo <= 0.f)
	{
		lilBoo.Update();

		entitiesTime += dt;

		float factorLilBoo = Maths::Clamp(entitiesTime / DURATION_LILBOO_MOVEMENTS, 0, 1);
		float easingLilBoo = cos(2 * PI * factorLilBoo + PI) / 2 + 0.5f;
		sf::Vector2f newPositionLilBoo = Maths::InterpolateVector(LILBOO_POSITION_1, LILBOO_POSITION_2, easingLilBoo);
		lilBoo.setScale((factorLilBoo < 0.5f ? -0.75f : 0.75f), 0.75f);
		lilBoo.setPosition(newPositionLilBoo + sf::Vector2f(0.f, sin(4 * totalTime) * 20));

		float factorBigBoo = Maths::Clamp(entitiesTime / DURATION_BIGBOO_MOVEMENTS, 0, 1);
		float inter = (0.5f - factorBigBoo);
		float easingBigBoo = 1 - (inter * inter * inter * inter) * 16;
		sf::Vector2f newPositionBigBoo = Maths::InterpolateVector(BIGBOO_POSITION_1, BIGBOO_POSITION_2, easingBigBoo);
		bigBoo.setPosition(newPositionBigBoo);

		if (entitiesTime >= DURATION_LILBOO_MOVEMENTS)
		{
			waitTimeLilboo = Maths::PreciseRand(WAIT_LILBOO_MAX, WAIT_LILBOO_MIN);
			entitiesTime = 0.f;
		}
	}
}
