#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "extensions/cocos-ext.h"

class loginscene :public cocos2d::Layer
{
private:
	cocos2d::Sprite*bg;
	cocos2d::Sprite*noc;
	cocos2d::ui::TextField* usernameInput;

	void createBackground();
	void initUserBox();

	void menuBackCallback(cocos2d::Ref* pSender);
	//void checkboxSelectedEvent(cocos2d::Ref* pSender);

public:

	static cocos2d::Scene * createScene();
	virtual bool init();
	CREATE_FUNC(loginscene);

};