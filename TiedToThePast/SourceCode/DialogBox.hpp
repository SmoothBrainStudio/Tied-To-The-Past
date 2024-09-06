#ifndef DIALOGBOX_HPP
#define DIALOGBOX_HPP

#include "DialogText.hpp"
#include "GameData.hpp"
#include "KeyBoard.hpp"

static constexpr float MARGIN_X = 32.f;
static constexpr float MARGIN_Y = 32.f;
static constexpr float ARROW_SIZE = 66.f;

class Dialog: public sf::Drawable, public sf::Transformable
{
public:
	Dialog();
	Dialog(const std::string& _path);
	~Dialog();

	enum class ArrowDirection
	{
		LEFT,
		RIGHT
	};

	void PollEvent(sf::Event& _event);
	void Update();
	void LoadFromFile(const std::string& _path);
	void Reset();

	void ChangeDirection();
	void ChangeDirection(const ArrowDirection& _dir);

	void StartDialog();
	void AddParagraph(const Paragraph& _paragraph);
	void NextParagraph();
	const bool& IsPlaying();
	const bool& IsFinish();
	const sf::Vector2f GetSize(void);
	void SetDialogDisappearing(const bool& _bool);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void AdaptToParagraph();
	void BoxUpdate();

	bool isPlaying;
	bool isFinish;
	ArrowDirection curArrowDir = ArrowDirection::LEFT;
	int curParagraph = 0;

	std::vector<Paragraph> paragraphs;

	sf::RectangleShape branch;
	sf::RectangleShape box;

	bool drawBox;
	bool dialogDisappearing = true;
};

#endif // !DIALOGBOX_HPP
