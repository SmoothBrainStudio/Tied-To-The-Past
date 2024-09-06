#ifndef EVENTABLE_HPP
#define EVENTABLE_HPP

#include "Common.hpp"

class Eventable
{
public:
	Eventable() = default;
	virtual ~Eventable();

    void SetOnClickFct(std::function<void()>&& _onClick) { clickCallBack = _onClick; };
    void SetOnClickReleaseFct(std::function<void()>&& _onRelease) { clickReleaseCallBack = _onRelease; };
    void SetWhileClickFct(std::function<void()>&& _whileClick) { whileClickCallBack = _whileClick; };
    void SetOnOverFct(std::function<void()>&& _onOver) { overCallBack = _onOver; };
    void SetOnOutFct(std::function<void()>&& _onOut) { outCallBack = _onOut; };
    void SetOnChangeFct(std::function<void()>&& _onChange) { changeCallBack = _onChange; };

    virtual void CallOnClick(bool _isClick = true);
    virtual void CallOnReleaseClick(bool _isRelease = true);
    virtual void CallWhileClick(bool _isClick = true);
    virtual void CallOnOver(bool _isOver = true);
    virtual void CallOnOut(bool _isOut = true);
    virtual void CallOnChange(bool _isChange = true);

protected:
    std::function<void()> clickCallBack = []() -> void {};
    std::function<void()> clickReleaseCallBack = []() -> void {};
    std::function<void()> whileClickCallBack = []() -> void {};
    std::function<void()> overCallBack = []() -> void {};
    std::function<void()> outCallBack = []() -> void {};
    std::function<void()> changeCallBack = []() -> void {};

};

#endif // !EVENTABLE_HPP
