#include "DialogText.hpp"

Paragraph::Paragraph(const Json::Value& _data)
{
    line text;
    sf::Text part;

    part.setFont(FontsManager::Get()["DEFAULT"]);
    part.setCharacterSize(_data["paragraph"]["size"].asInt());
    SetOutlined(part, _data["paragraph"]["outline"]["thickness"].asInt(), HexToRgba(_data["paragraph"]["outline"]["color"].asString()));
    boxType = _data["paragraph"]["box"].asString();

    for (auto &_subData : _data["content"])
    {
        sf::Text nPart(part);

        nPart.setString(_subData["value"].asString());
        nPart.setFillColor(HexToRgba(_subData["color"].asString()));
        nPart.setStyle(
            sf::Text::Style::Bold * _subData["bold"].asBool() |
            sf::Text::Style::Italic * _subData["italic"].asBool() |
            sf::Text::Style::Underlined * _subData["underlined"].asBool() |
            sf::Text::Style::StrikeThrough* _subData["strikethrough"].asBool()
        );

        text.push_back(nPart);
    }

    align = (TextAlign)_data["paragraph"]["align"].asInt();
    animation.timeByCharacter = _data["paragraph"]["animation"]["timeByCharacter"].asInt() / 1000.f;

    animation.dialogKey = _data["paragraph"]["sound"].asString();

    lines = splitText(text);
    InitPosition();
}

Paragraph::~Paragraph()
{

}

void Paragraph::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    int count = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            std::string curWord;
            for (int k = 0; k < lines[i][j].getString().getSize(); k++)
            {
                ++count;
                curWord += lines[i][j].getString()[k];
                if (count > animation.curTime / animation.timeByCharacter)
                {
                    sf::Text copy(lines[i][j]);
                    copy.setString(curWord);
                    target.draw(copy, states);
                    return;
                }
            }
            target.draw(lines[i][j], states);
        }
    }
}

void Paragraph::InitPosition()
{
    float lastPosY = getPosition().y;

    for (int i = 0; i < lines.size(); i++)
    {
        float maxHeight = GetMaxLineHeight();
        float lastPosX = getPosition().x;

        switch (align)
        {
        case Right:
            lastPosX += GetMaxLineWidth() - GetLineWidth(i);
            break;
        case Center:
            lastPosX += GetMaxLineWidth() / 2.f - GetLineWidth(i) / 2.f;
            break;
        default:
            break;
        }

        for (int j = 0; j < lines[i].size(); j++)
        {
            lines[i][j].setPosition(sf::Vector2f(lastPosX, lastPosY));

            lastPosX += lines[i][j].getGlobalBounds().width;
        }
        lastPosY += maxHeight + lineSpacing;
    }
}

void Paragraph::Update()
{
    if (animation.dialogKey.empty()) return;

    if (Audio::Get().GetSound(animation.dialogKey).getStatus() != sf::Sound::Status::Playing && !IsAnimationEnd())
    {
        Audio::Get().SetSoundPitch(animation.dialogKey,((rand() % 25) + 75) / 100.f);
        Audio::Get().PlaySound(animation.dialogKey);
    }
}

void Paragraph::StopAnimation()
{
    animation.curTime = animation.timeByCharacter * GetCharacterSize();
}

void Paragraph::AddTime(const float& _toAdd)
{
    animation.curTime += _toAdd;
}

const bool Paragraph::IsAnimationEnd()
{
    return (animation.curTime / animation.timeByCharacter >= GetCharacterSize());
}

void Paragraph::ResetAnimation()
{
    animation.curTime = 0.f;
}

sf::Vector2f Paragraph::GetSize()
{
    sf::Vector2f result;

    result.x = GetMaxLineWidth();
    result.y = lineSpacing * lines.size();
    for (int i = 0; i < lines.size(); i++)
    {
        result.y += GetLineHeight(i);
    }

    return result;
}

float Paragraph::GetMaxLineWidth()
{
    float maxWidth = 0.f;
    for (int i = 0; i < lines.size(); i++)
    {
        float curWidth = GetLineWidth(i);

        if (maxWidth < curWidth)
        {
            maxWidth = curWidth;
        }
    }
    return maxWidth;
}

float Paragraph::GetMaxLineHeight()
{
    float maxHeight = 0.f;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            float curHeight = lines[i][j].getGlobalBounds().height;

            if (maxHeight < curHeight)
            {
                maxHeight = curHeight;
            }
        }
    }
    return maxHeight;
}

const float Paragraph::GetLineWidth(const int& line)
{
    float width = 0.f;
    for (int j = 0; j < lines[line].size(); j++)
    {
        width += lines[line][j].getGlobalBounds().width;
    }
    return width;
}

const float Paragraph::GetLineHeight(const int& line)
{
    float maxHeight = 0.f;
    for (int i = 0; i < lines[line].size(); i++)
    {
        float curHeight = lines[line][i].getGlobalBounds().height;

        if (maxHeight < curHeight)
        {
            maxHeight = curHeight;
        }
    }
    return maxHeight;
}

const float& Paragraph::GetLineSpacing()
{
    return lineSpacing;
}

const int Paragraph::GetLineSize()
{
    return (int)lines.size();
}

const int Paragraph::GetCharacterSize()
{
    int result = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].size(); j++)
        {
            for (int k = 0; k < lines[i][j].getString().getSize(); k++)
            {
                ++result;
            }
        }
    }
    return result;
}

const std::string& Paragraph::GetBoxType()
{
    return boxType;
}

std::vector<line> splitText(line& _text)
{
    int totalLine = 1;
    int curLine = 0;
    std::vector<line> lines;

    for (int i = 0; i < _text.size(); i++)
    {
        std::string value = _text[i].getString().toAnsiString();

        // Check if the text has a line return
        int hasReturn = countOneLetter(value, '\n');
        totalLine += hasReturn;
        lines.resize(totalLine);

        if (!hasReturn)
        {
            // If the text hasn't \n, it is add to the line
            lines[curLine].push_back(_text[i]);
        }
        else
        {
            // Else the text is split to add other lines
            sf::String newString;
            sf::Text tempText(_text[i]);
            for (int j = 0; j < value.size(); j++)
            {
                if (value[j] != '\n')
                {
                    newString += value[j];
                }

                if (value[j] == '\n' || j == value.size() - 1)
                {
                    tempText.setString(newString);
                    lines[curLine].push_back(tempText);

                    if (value[j] == '\n') curLine++;
                    newString.clear();
                }
            }
        }
    }

    return lines;
}
