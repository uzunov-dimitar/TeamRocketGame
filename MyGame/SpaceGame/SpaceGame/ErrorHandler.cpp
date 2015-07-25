#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() : mFont(IND_Font::newFont()), mText2d(IND_Entity2d::newEntity2d())
{
}

IND_Font * ErrorHandler::getFont() const
{
	return mFont;
}

void ErrorHandler::setFont(IND_Font * mFont)
{
	this->mFont = mFont;
}

IND_Entity2d * ErrorHandler::getText2d() const
{
	return mText2d;
}

void ErrorHandler::setText2d(IND_Entity2d * mText2d)
{
	this->mText2d = mText2d;
}

void ErrorHandler::initialize(CIndieLib * mI)
{
	mI->_fontManager->add(getFont(), "resources/font_small.png", "resources/font_small.xml", IND_ALPHA, IND_32);

	mI->_entity2dManager->add(getText2d());

	getText2d()->setFont(getFont());
}

void ErrorHandler::writeError(float posX, float posY,string label, string value)
{
	getText2d()->setPosition(posX, posX, 0);
	getText2d()->setText((label+":" + value).c_str());
}

void ErrorHandler::writeError(float posX, float posY, string label, float value)
{
	char* text = new char[100];
	sprintf(text, "%f", value);
	writeError(posX, posY, label, text);
	delete[] text;
}

ErrorHandler::~ErrorHandler()
{
	getFont()->destroy();
	getText2d()->destroy();
}
