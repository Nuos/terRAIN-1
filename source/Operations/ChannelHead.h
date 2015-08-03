#ifndef CHANNELHEAD_H
#define CHANNELHEAD_H

#include "RasterPosition.h"
#include "rastermatrix.h"
#include <vector>
#include <map>

namespace TR
{

struct ChannelHeadHistoryRecord
{
	RasterPosition pos;
	double time;
};

typedef std::vector<ChannelHeadHistoryRecord> ChannelHeadHistory;

class ChannelHead
{
private:
	size_t _id;
	RasterPosition _pos;
	double _generationTime;
	double _ereaseTime;
	double _lastMoveTime;
	ChannelHeadHistory _history;
public:
	ChannelHead(size_t id, RasterPosition pos, double generationTime);

	size_t getID() const;
	RasterPosition getPosition() const;
	double getGenerationTime() const;
	double getEreaseTime() const;
	double getLastMoveTime() const;
	const ChannelHeadHistory & history() const;
	bool isExist() const;
	void erease(double ereaseTime);

	void moveTo(RasterPosition pos, double time);
};

typedef std::map<int, ChannelHead> ChannelHeadMap; 

class ChannelHeadTracker
{
private:
	size_t _sizeX;
	size_t _sizeY;
	int _lastID;
	ChannelHeadMap _channelHeadMap;
	IntRasterMx _channelHeads;
public:
	ChannelHeadTracker(size_t sizeX, size_t sizeY);
	int lastID() const;
	const ChannelHeadMap & channelHeadMap() const;
	const IntRasterMx & channelHeads() const;
	bool track(const DblRasterMx & currentChannelHeads, double time);
};

}

#endif