#ifndef RENDERWINDOWPARAMS_H
#define RENDERWINDOWPARAMS_H

namespace VISUALIZATION
{

class RenderWindowParams
{
private:
	int _sizeX;
	int _sizeY;
public:
	RenderWindowParams(int sizeX, int sizeY);

	int getSizeX() const;
	int getSizeY() const;
};

}

#endif