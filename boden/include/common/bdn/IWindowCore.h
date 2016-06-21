#ifndef BDN_IWindowCore_H_
#define BDN_IWindowCore_H_

#include <bdn/IViewCore.h>

namespace bdn
{


/** The core for a top level window.*/
class IWindowCore : BDN_IMPLEMENTS IViewCore
{
public:

	/** Changes the window's title.*/
	virtual void setTitle(const String& title)=0;


	/** Returns the area where the Window's content window
		should be.*/
	virtual Rect getContentArea()=0;


	/** Calculates the size that the whole window from the size of the content area.
		This usually adds sizes for the window border (if it has any), title bar, menu bar
		and the like.
	*/
	virtual Size calcWindowSizeFromContentAreaSize(const Size& contentSize)=0;


	/** Calculates the size of the content area from the size of the whole window.
	
		This is the inverse of calcWindowSizeFromContentAreaSize().	
	*/
	virtual Size calcContentAreaSizeFromWindowSize(const Size& windowSize)=0;


	/** Returns the area of the screen that can be used by windows.
		That excludes taskbars, sidebars and the like (if they are always visible).
		The returned rect applies only to the screen that the window is currently on.
		Other screens can have different window areas.*/
	virtual Rect getScreenWorkArea()=0;
	
};


}

#endif
