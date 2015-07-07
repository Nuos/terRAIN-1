#include "ChannelHead.h"

namespace TR
{


ChannelHead::ChannelHead(size_t id, RasterPosition pos, double generationTime) :
	_id(id), _pos(pos), _generationTime(generationTime), _ereaseTime(-1.0), _lastMoveTime(generationTime)
{
	ChannelHeadHistoryRecord record;
	record.pos = pos;
	record.time = generationTime;
	_history.push_back(record);
}

size_t ChannelHead::getID() const
{
	return _id;
}

RasterPosition ChannelHead::getPosition() const
{
	return _pos;
}
	
double ChannelHead::getGenerationTime() const
{	
	return _generationTime;
}
	
double ChannelHead::getEreaseTime() const
{
	return _ereaseTime;
}
	
double ChannelHead::getLastMoveTime() const
{
	return _lastMoveTime;
}
	
const ChannelHeadHistory & ChannelHead::history() const
{
	return _history;
}


bool ChannelHead::isExist() const
{
	return ( _ereaseTime > _generationTime);
}

void ChannelHead::moveTo(RasterPosition pos, double time)
{
	ChannelHeadHistoryRecord record;
	record.pos = pos;
	record.time = time;
	_history.push_back(record);

	_pos = pos;
	_lastMoveTime = time;
}

void ChannelHead::erease(double ereaseTime)
{
	_ereaseTime = ereaseTime;
}

}