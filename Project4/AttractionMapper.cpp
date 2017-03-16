#include "provided.h"
#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> Att_hld; 
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
	Att_hld.clear();
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	int size = ml.getNumSegments(); //total number of street segements
	//cerr << "number of segments" << size << endl;
	for (int i = 0; i < size; i++) //going through  each stree 
	{
		//cerr << "processing " << i << " segament" << endl;
		StreetSegment st_ptr; 
		ml.getSegment(i, st_ptr);

		int att_size = st_ptr.attractions.size(); //att_size is 0 if there is no attraction 
		int walk = 0;
		while (walk < att_size) //skip when there is no attraction 
		{
			Att_hld.associate(st_ptr.attractions[walk].name,st_ptr.attractions[walk].geocoordinates); //add to the mymap 
			walk++;
		}
	}

}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	const GeoCoord* gc_ptr;
	gc_ptr = Att_hld.find(attraction); //find the geo coord according to the name of teh attraction 
	if (gc_ptr != nullptr) //if there is one
	{
		gc = *gc_ptr;
		return true; 
	}
	else
		return false; 
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
