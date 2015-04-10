#ifndef TRPOINT3D_H
#define TRPOINT3D_H

class trPoint3d
{
private:
	double _x;
	double _y;
	double _z;
public:
	trPoint3d();
	trPoint3d(double x, double y, double z);
	void setX(double x);
	void setY(double y);
	void setZ(double z);
	void set(double x, double y, double z);

	double getX() const;
	double getY() const;
	double getZ() const;
};

#endif