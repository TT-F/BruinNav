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
		string line;
		//getline(infile, start_lat, ',');
		//infile.ignore();
		////cerr << "entering load function CP 2" << endl;
		//getline(infile, start_long, ' ');
		//if (start_long.front() == ' ')
		//	cerr << start_long.front() << endl;
		//getline(infile, end_lat, ',');
		//infile.ignore();
		//getline(infile, end_long);
		//cerr << "entering load function CP 3" << endl;
		getline(infile, line);
		for (unsigned i = 0; i < line.size();i++)
		{
			if (line.at(i) == ',')
			{
				start_lat = line.substr(0, i);
				line.erase(0, i+1);
				break;
			}
		}
		for (unsigned i = 0; i < line.size();i++)
		{
			if (line.at(i) == ' ' && i != 0)
			{
				start_long = line.substr(0, i);
				line.erase(0, i+1);
				break;
			}
		}
		if (*start_long.begin() == ' ')
			start_long.erase(start_long.begin());

		for (unsigned i = 0; i < line.size();i++)
		{
			if (line.at(i) == ',' )
			{
				end_lat = line.substr(0, i);
				line.erase(0, i + 1);
				break;
			}
		}
		end_long = line;
		if (*end_long.begin() == ' ')
			end_long.erase(end_long.begin());
		int count;
		infile >> count;
		//cerr << "Initial count" << count << endl;
		infile.ignore(1000, '\n');
		//cerr << "entering load function CP 4" << endl;
		string att_name, att_lat, att_long;
		while (count!= 0)
		{
			//cerr << "entering load function CP entering while" << endl;
			line = "";
			getline(infile, line);
			for (unsigned i = 0; i < line.size();i++)
			{
				if (line.at(i) == '|')
				{
					att_name = line.substr(0, i);
					line.erase(0, i + 1);
					break;
				}
			}
			for (unsigned i = 0; i < line.size();i++)
			{
				if (line.at(i) == ',')
				{
					att_lat = line.substr(0, i);
					line.erase(0, i + 1);
					break;
				}
			}
			att_long = line;
			if (*att_long.begin() == ' ')
				att_long.erase(att_long.begin());
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
	if (segNum >= m_seg.size()) //cno
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
