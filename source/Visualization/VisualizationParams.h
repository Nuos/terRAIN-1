#ifndef VISUALIZATIONPARAMS_H
#define VISUALIZATIONPARAMS_H

namespace VISUALIZATION
{

class VisualizationParams
{
protected:
	int _renderWindowSizeX;
	int _renderWindowSizeY;
public:
	VisualizationParams();
	VisualizationParams(int renderWindowSizeX, int renderWindowSizeY);

	int getRenderWindowSizeX() const;
	int getRenderWindowSizeY() const;

	void setRenderWindowSizeX( int renderWindowSizeX );
	void setRenderWindowSizeY( int renderWindowSizeY );

};

}
#endif