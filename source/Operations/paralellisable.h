#ifndef PARALELLISABLE
#define PARALELLISABLE

namespace TR
{

class paralellisable
{

public:
	paralellisable(){}
	virtual void run()=0;
	virtual paralellisable * clone() const = 0;

};

}

#endif

