#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	//vector<StreetSegment> seg_map;
	MyMap<string, vector<StreetSegment*>> My_segment;
	vector<StreetSegment*> segments_loader;
	void process(GeoCoord input, StreetSegment* ss);
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
	int size = segments_loader.size();
	for (int i = 0; i < size;i++)
	{
		delete segments_loader[i];
	}
	
	My_segment.clear();
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0;i < ml.getNumSegments();i++)
	{
		StreetSegment steet_seg;
		ml.getSegment(i, steet_seg);
		StreetSegment* st_ptr = new StreetSegment(steet_seg);
		segments_loader.push_back(st_ptr);
		process(steet_seg.segment.start, st_ptr);
		process(steet_seg.segment.end, st_ptr);
		for (int k = 0; k < steet_seg.attractions.size();k++)
		{
			GeoCoord ac = steet_seg.attractions[k].geocoordinates;
			if (!(ac == steet_seg.segment.start || ac == steet_seg.segment.end))
				process(ac, st_ptr);
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	string loc = geo_to_num(gc);
	vector<StreetSegment> segments;
	const vector<StreetSegment*>* ptr = My_segment.find(loc);
	if (ptr != nullptr)
	{
		for (int i = 0;i < ptr->size();i++)
			segments.push_back(*(*ptr)[i]);
	}
	return segments;
}

void SegmentMapperImpl::process(GeoCoord input, StreetSegment * ss)
{
	string inst = geo_to_num(input);
	vector<StreetSegment*>* sp = My_segment.find(inst);
	if (sp != nullptr)
		sp->push_back(ss);
	else
		My_segment.associate(inst, { ss });
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
