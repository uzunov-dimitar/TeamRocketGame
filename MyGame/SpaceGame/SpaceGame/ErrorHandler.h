#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include "CIndieLib.h"
#include "IND_Font.h"
#include "IND_Entity2d.h"

class ErrorHandler {
private:
	bool initialized;
	IND_Font* mFont;
	IND_Entity2d* mText2d;
public:
	ErrorHandler();

	bool isInitialized()const;
	void setInitialized(bool);
	IND_Font* getFont() const;
	void setFont(IND_Font*);
	IND_Entity2d* getText2d() const;
	void setText2d(IND_Entity2d*);

	void initialize(CIndieLib*);
	void writeError(float, float, string, string);
	void writeError(float, float, string, float);

	virtual ~ErrorHandler();
};
#endif
