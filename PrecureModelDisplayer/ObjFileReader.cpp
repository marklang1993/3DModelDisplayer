#include "ObjFileReader.h"
#include "PrecureModelDisplayerException.h"

#include <fstream>

using namespace std;

ObjFileReader::ObjFileReader(string filename)
{
	ifstream objFile;
	objFile.open(filename);
	//Check file status
	if (objFile.fail())
	{
		throw PMD_FailedReadObjFile();
	}

	while (!objFile.eof())
	{
		//Read oneline
		string oneline;
		getline(objFile, oneline);

		//Parse oneline
		stringstream ss;
		ss << oneline;
		//Get the 1st token
		string firstToken;
		ss >> firstToken;
		//Check the type of token
		if (firstToken == "v")
		{
			//Vertex
			processVertex_(ss);
		}
		else if (firstToken == "f")
		{
			//Index (Face)
			processIndex_(ss);
		}
	}
	//Finished
	objFile.close();
}

void ObjFileReader::ParseVertex(vector<Vertex>** vertex)
{
	*vertex = &vertex_;
}

void ObjFileReader::ParseIndex(vector<UINT>** index)
{
	*index = &index_;
}

void ObjFileReader::processVertex_(stringstream& str)
{
	Vertex vertex;
	//Initialize Position
	str >> vertex.Position.x;
	str >> vertex.Position.y;
	str >> vertex.Position.z;
	//Initialize Color
	vertex.Color = *(DirectX::XMFLOAT4*)&Colors::Grey;

	vertex_.push_back(vertex);
}

void ObjFileReader::processIndex_(stringstream& str)
{
	string index;
	//Get x, y, z coordinates
	for (int i = 0; i < 3; i++)
	{
		//Get index Token
		str >> index;
		//Process and Store that index
		index_.push_back(seperateIndex(index));
	}
}

UINT ObjFileReader::seperateIndex(string& indexToken)
{
	stringstream ssInput;
	stringstream ssOutput;
	//Read a char until reach end of the stream OR char '/'
	ssInput << indexToken;
	while (!ssInput.eof())
	{
		char c;
		ssInput >> c;
		if (c == '/')
		{
			break;
		}
		else
		{
			ssOutput << c;
		}
	}
	//Output to an Integer
	UINT index;
	ssOutput >> index;

	return index;
}