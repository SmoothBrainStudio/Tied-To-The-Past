#ifndef TEXT_HPP
#define TEXT_HPP

#include "Common.hpp"
#include "GameClock.hpp"
#include "Text.hpp"

#define LINE_SPACING 8
#define DEFAULT_SIZE_TEXT 24
#define DEFAULT_COLOR_TEXT "#ffffff"
#define DEFAULT_FONT_TEXT "Assets/arial.ttf"

enum TextAlign
{
    Left,
    Right,
    Center,
    Justify
};

struct TextAnimation
{
    float timeByCharacter = 0.f;
    float curTime = 0.f;
    std::string dialogKey;
};

typedef std::vector<sf::Text> line;

class Paragraph: public sf::Transformable, public sf::Drawable
{
public:
    Paragraph() = default;
    Paragraph(const Json::Value& _data);
    ~Paragraph();

    void InitPosition();
    void Update();

    void StopAnimation();
    void AddTime(const float& _toAdd);
    const bool IsAnimationEnd();
    void ResetAnimation();

    sf::Vector2f GetSize();

    float GetMaxLineWidth();
    float GetMaxLineHeight();

    const float GetLineWidth(const int& line);
    const float GetLineHeight(const int& line);

    const float& GetLineSpacing();
    const int GetLineSize();
    const int GetCharacterSize();
    const std::string& GetBoxType();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<line> lines;
    float lineSpacing = LINE_SPACING;
    TextAlign align = Center;
    TextAnimation animation;
    std::string boxType;
};

std::vector<line> splitText(line& _text);

#endif // !TEXT_HPP
