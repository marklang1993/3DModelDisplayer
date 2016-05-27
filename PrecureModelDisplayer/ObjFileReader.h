#ifndef OBJFILEREADER_H
#define OBJFILEREADER_H

#include "d3d11.h"
#include "Library.h"

#include <string>
#include <vector>
#include <sstream>


class ObjFileReader
{
public:
	ObjFileReader(std::string filename);	// Constructor - Initilize Obj File by filename

	void ParseVertex(std::vector<Vertex>** vertex);		// Get an array of Vertex Data
	void ParseIndex(std::vector<UINT>** index);			// Get an array of Index Data

private:
	std::vector<Vertex> vertex_;	// Store one line of vertex data (marked by "v")
	std::vector<UINT> index_;		// Store one line of index data (marked by "f")

	void processVertex_(std::stringstream& str);
	void processIndex_(std::stringstream& str); 
	UINT seperateIndex(std::string& indexToken);
};

#endif