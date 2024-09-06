#include "DialogBox.hpp"

Dialog::Dialog() : drawBox(false), isPlaying(false), isFinish(false)
{
    TexturesManager& texMng = TexturesManager::Get();
    box.setTexture(&texMng["DIALOG_BOX"], true);
    branch.setSize(sf::Vector2f(ARROW_SIZE, ARROW_SIZE));
}

Dialog::Dialog(const std::string& _path) : drawBox(false), isPlaying(false), isFinish(false)
{
    LoadFromFile(_path);

    TexturesManager& texMng = TexturesManager::Get();
    box.setTexture(&texMng["DIALOG_BOX"], true);
    branch.setSize(sf::Vector2f(ARROW_SIZE, ARROW_SIZE));
}

Dialog::~Dialog()
{
}

void Dialog::AdaptToParagraph()
{
    sf::FloatRect branchRect = branch.getGlobalBounds();
    sf::Vector2f paraSize = paragraphs[curParagraph].GetSize();

    box.setSize(paraSize + sf::Vector2f(MARGIN_X, MARGIN_Y));
    sf::FloatRect boxRect = box.getGlobalBounds();

    setOrigin(0, boxRect.height);

    box.setOrigin(0, boxRect.height);
    box.setPosition(getOrigin() - sf::Vector2f(0, (ARROW_SIZE - 2)));

    branch.setOrigin(0, branchRect.height);
    branch.setPosition(getOrigin());

    paragraphs[curParagraph].setOrigin(paraSize.x / 2.f, paraSize.y / 2.f);
    paragraphs[curParagraph].ResetAnimation();
    paragraphs[curParagraph].setPosition(boxRect.width / 2.f, boxRect.height / 2.f - (ARROW_SIZE - 2));

    if (curArrowDir == ArrowDirection::LEFT) paragraphs[curParagraph].setScale(1, 1);
    else paragraphs[curParagraph].setScale(-1, 1);
}

void Dialog::BoxUpdate()
{
    TexturesManager& texMng = TexturesManager::Get();
    std::string boxType = paragraphs[curParagraph].GetBoxType();

    if (boxType != "")
    {
        branch.setTexture(&texMng[boxType]);
        AdaptToParagraph();
        drawBox = true;
    }
    else
    {
        drawBox = false;
    }
}
 
void Dialog::ChangeDirection()
{
    curArrowDir = (curArrowDir == ArrowDirection::LEFT ? ArrowDirection::RIGHT : ArrowDirection::LEFT);
    sf::Vector2f curScale = getScale();
    setScale(-curScale.x, curScale.y);

    if (curArrowDir == ArrowDirection::LEFT) paragraphs[curParagraph].setScale(1, 1);
    else paragraphs[curParagraph].setScale(-1, 1);
}

void Dialog::ChangeDirection(const ArrowDirection& _dir)
{
    if (curArrowDir != _dir)
    {
        curArrowDir = _dir;
        sf::Vector2f curScale = getScale();
        setScale(-curScale.x, curScale.y);
    }

    if (curArrowDir == ArrowDirection::LEFT) paragraphs[curParagraph].setScale(1, 1);
    else paragraphs[curParagraph].setScale(-1, 1);
}

void Dialog::StartDialog()
{
    curParagraph = 0;
    isPlaying = true;
    isFinish = false;

    BoxUpdate();
}

void Dialog::AddParagraph(const Paragraph& _paragraph)
{
    paragraphs.push_back(_paragraph);
}

void Dialog::NextParagraph()
{
    if (curParagraph < paragraphs.size() - 1)
    {
        curParagraph++;
        paragraphs[curParagraph].ResetAnimation();

        BoxUpdate();
    }
    else
    {
        isPlaying = false;
        isFinish = true;
    }
}

const bool& Dialog::IsPlaying()
{
    return isPlaying;
}

const bool& Dialog::IsFinish()
{
    return isFinish;
}

const sf::Vector2f Dialog::GetSize(void)
{
    return paragraphs[curParagraph].GetSize();
}

void Dialog::SetDialogDisappearing(const bool& _bool)
{
    dialogDisappearing = _bool;
}

void Dialog::PollEvent(sf::Event& _event)
{
    if (!isPlaying) return;

    if ((_event.type == sf::Event::KeyPressed || _event.type == sf::Event::JoystickButtonPressed) && dialogDisappearing)
    {
        if (paragraphs[curParagraph].IsAnimationEnd())
        {
            NextParagraph();
        }
        else
        {
            paragraphs[curParagraph].StopAnimation();
        }
    }
}

void Dialog::Update()
{
    if (!isPlaying) return;

    // Update animation
    if (!paragraphs[curParagraph].IsAnimationEnd())
    {
        paragraphs[curParagraph].AddTime(GameClock::Get().GetDeltaTime() * 2.5f);
    }

    // Update current paragraph
    paragraphs[curParagraph].Update();
}

void Dialog::LoadFromFile(const std::string& _path)
{
    std::string path = "Contents/Data/Dialogs/" + _path;
    std::ifstream file(path);

    if (!file.is_open())
    {
        throw "The file: \"" + path + "\" can't be open";
    }

    std::string data;
    file >> data;

    if (data != "IS_DIALOG")
    {
        throw "The file: \"" + path + "\" is corrumpted or not a dialog text";
    }

    while (!file.eof())
    {
        file >> data;
        AddParagraph(Paragraph(JsonManager::LoadFromFile(data)["text"]));
    }
}

void Dialog::Reset()
{
    isPlaying = false;
    isFinish = false;
}

void Dialog::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    if (isPlaying)
    {
        if (drawBox)
        {
            target.draw(branch, states);
            target.draw(box, states);
        }
        target.draw(paragraphs[curParagraph], states);
    }
}
