#ifndef CIRCULAR_SELECT__HPP
#define CIRCULAR_SELECT__HPP
#include "Definitions.hpp"
#include "PieChart.hpp"
#include "AnimatedSprite.hpp"
#include "Audio.hpp"

class Player;

class CircularSelect : public sf::Drawable, public sf::Transformable
{
public:
	CircularSelect(sf::Texture& _texture, const int& _nbPieces, const std::vector<std::string>& _tiesName);
	~CircularSelect();

	void PollEvent(sf::Event& _event);
	void Update(sf::RenderWindow& _window);

	const bool& GetIsVisible(void);
	void SetIsVisible(const bool& _visible);
	const int& GetTieIndex(void) const;
	void SetTieIndex(const int& _index);

	void ChangeTieTextures(std::shared_ptr<Player>& _player);

	struct Piece
	{
		AnimatedSprite sprite;
		bool isSelected = false;
	};
	std::vector<Piece> circularPieces;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void PlaySwapingPieceSound(void);



	sf::Sprite wheelBG;

	int tieIndex = -1;
	
	std::vector<sf::Sprite> tiesSpriteVect;
	bool isVisible = true;
	float angleWithDevice = 0.f;
	unsigned short currentSelectedPiece = 0;
	unsigned short lastSelectedPiece = 0;
	unsigned short nbTiesAvailable = 0;

	Audio& audio = Audio::Get();
};

#endif // !CIRCULAR_SELECT__HPP



