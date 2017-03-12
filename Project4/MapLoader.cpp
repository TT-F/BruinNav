#include "provided.h"
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment*> m_seg; 
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
	int size = m_seg.size(); //cno
	for (int i = 0; i < size; i++)
		delete m_seg[i];
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);    // infile is a name of our choosing
	if (!infile)		        // Did opening the file fail?
	{
		cerr << "Error: Cannot open data.txt!" << endl; //CNO 
		return false; 
	}
	//cerr << "entering load function" << endl;
	string name; 
	while (getline(infile, name))
	{
		//cerr << "entering load function CP 1" << endl;
		StreetSegment* temp_seg= new StreetSegment;
		string start_lat, start_long, end_lat, end_long; 
		getline(infile, start_lat, ',');
		infile.ignore();
		//cerr << "entering load function CP 2" << endl;
		getline(infile, start_long, ' ');
		getline(infile, end_lat, ',');
		getline(infile, end_long);
		//cerr << "entering load function CP 3" << endl;
		int count;
		infile >> count;
		//cerr << "Initial count" << count << endl;
		infile.ignore(10000, '\n');
		//cerr << "entering load function CP 4" << endl;
		string att_name, att_lat, att_long;
		while (count!= 0)
		{
			//cerr << "entering load function CP entering while" << endl;
			getline(infile, att_name, '|');
			getline(infile, att_lat, ',');
			infile.ignore();
			getline(infile, att_long);
			//cerr << "entering load function CP W 1" << endl;
			GeoCoord att_cord(att_lat, att_long);
			Attraction att;
			att.name = att_name;
			att.geocoordinates = att_cord;
			temp_seg->attractions.push_back(att);
			count--;
			//cerr << "entering load function CP W 2" << endl;
			//cerr << "left count" << count << endl;
		}
		//cerr << "entering load function CP 5" << endl;
		GeoCoord m_start(start_lat,start_long);
		//cerr << "entering load function CP 6" << endl;
		GeoCoord m_end(end_lat,end_long);
		GeoSegment g_seg(m_start, m_end);
		
		temp_seg->streetName = name;
		temp_seg->segment = g_seg;
		
		m_seg.push_back(temp_seg);
		//cerr << "entering load function CP END" << endl;
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return m_seg.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum >= m_seg.size() || segNum <0) //cno
		return false;

	seg = *(m_seg[segNum]);
	return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
