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


ChannelHeadTracker::ChannelHeadTracker(size_t sizeX, size_t sizeY) : 
	_sizeX(sizeX), _sizeY(sizeY), _lastID(0)
{
	int initVal = 0;
	_channelHeads.init(_sizeY, _sizeX, 1.0, origoBottomLeft, initVal);
}
	
int ChannelHeadTracker::lastID() const
{
	return _lastID;
}
	
const ChannelHeadMap & ChannelHeadTracker::channelHeadMap() const
{
	return _channelHeadMap;
}

const IntRasterMx & ChannelHeadTracker::channelHeads() const
{
	return _channelHeads;
}

bool ChannelHeadTracker::track(const DblRasterMx & currentChannelHeads, double time)
{
	DblRasterMx currentChannelHeadsCopy(currentChannelHeads);

	//first try to track the current channel heads:
	
	IntRasterMx::iterator iChannelHeads = _channelHeads.begin(),  endChannelHeads = _channelHeads.end();

	for (; iChannelHeads != endChannelHeads; ++iChannelHeads) {
		int id = *iChannelHeads;

		if (id > 0) {
			size_t row = iChannelHeads.getRow();
			size_t col = iChannelHeads.getCol();

			DblRasterMx::iterator iCurrentChannelHead = currentChannelHeadsCopy.getIteratorAt(row, col);
			
			// the channel head did not move
			if (*iCurrentChannelHead > 1e-6) {
				*iCurrentChannelHead = 0.0;
				continue;
			}

			// check if the channel head moved
			int n = 0;
			DblRasterMx::iterator::dCoords delta;
			delta._nDCol = 0;
			delta._nDRow = 0;
			for (unsigned char cc = 1; cc < 10; ++cc) {
				if (cc==5 || !iCurrentChannelHead.isValidItemByChainCode(cc))
					continue;
				double val = iCurrentChannelHead.chain_code(cc);
				if (val > 1e-6) {
					++n;
					delta = iCurrentChannelHead.getChainCodeDelta(cc);
					currentChannelHeadsCopy(row + delta._nDRow, col + delta._nDCol) = 0.0;
				}
			}
			
			if (n==1) { // the channel head moved, and there is only one direction
				*iChannelHeads = 0;
				RasterPosition new_pos(row + delta._nDRow, col + delta._nDCol); 
				_channelHeads(new_pos.getRow(), new_pos.getCol()) = id;
				_channelHeadMap[id].moveTo(new_pos, time);
			} else if (n==0) { // the channel head is ereased
				*iChannelHeads = 0;
				_channelHeadMap[id].erease(time);
			} else if (n > 1) { // the channel head moved, but there are multiple directions
				// not yet implemented
				return false;
			}

		}
	}
	
	// find the new channel heads
	DblRasterMx::iterator iCurrentChannelHead = currentChannelHeadsCopy.begin(), endCurrentChannelHead = currentChannelHeadsCopy.end();

	for (; iCurrentChannelHead != endCurrentChannelHead; ++iCurrentChannelHead) {
		if (*iCurrentChannelHead > 1e-6) {
			RasterPosition pos(iCurrentChannelHead.getRow(), iCurrentChannelHead.getCol());
			++_lastID;
			_channelHeadMap[_lastID] = ChannelHead(_lastID, pos, time);
		}
	}

	return true;
}

}