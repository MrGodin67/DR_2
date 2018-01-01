#include "stdafx.h"
#include "MapFileParser.h"
#include <fstream>
#include "Utils.h"
void MapFileParser::Parse()
{
	
	
	data1.layers.resize(numbLayers);
	std::vector<std::string> tokens;
	
	for (int i = 0; i < lines.size(); i++)
	{
		std::string str = lines[i];
		
		tokens.clear();
		Utils::Tokenize(&tokens, str, " ");
		if (tokens[0] == "maptileset")
		{
			mbstowcs_s(NULL, 
				data1.tileSetFilename, 
				tokens[1].size() + 1,
				tokens[1].c_str(), 
				_TRUNCATE);
		
		}
		if (tokens[0] == "entitytileset")
		{
			mbstowcs_s(NULL,
				data1.entitySetFilename,tokens[1].size() + 1,
				tokens[1].c_str(),
				_TRUNCATE);

		}
		if(tokens[0] == "tileswide")
			data1.width = atoi(tokens[1].c_str());
		if (tokens[0] == "tileshigh")
			data1.height = atoi(tokens[1].c_str());
		if (tokens[0] == "tilewidth")
			data1.cellWidth = atoi(tokens[1].c_str());
		if (tokens[0] == "tileheight")
			data1.cellHeight = atoi(tokens[1].c_str());
		if (tokens[0] == "drawheight")
			data1.drawHeight = atoi(tokens[1].c_str());
		if (tokens[0] == "drawwidth")
			data1.drawWidth = atoi(tokens[1].c_str());
		if (tokens[0] == "layer")
		{
			int layerIndex = atoi(tokens[1].c_str());
			int index = i + 1;
			vstring tempTokens;
			while (true)
			{
				std::string str2 = lines[index];
				tempTokens.clear();
				Utils::Tokenize(&tempTokens, str2, ",");
				for (int k = 0; k < tempTokens.size()-1; k++)
				{
					data1.layers[layerIndex].push_back(atoi(tempTokens[k].c_str()));
				}
				tempTokens.clear();
				Utils::Tokenize(&tempTokens, str2, " ");
				// found another layer so go back to original loop
				if (tempTokens[0] == "layer")
					break;
				index++;
				if (index >= lines.size())//end of file
				{
					return;
				}
			}
		}
	}
	
}

void MapFileParser::Load(const std::string & filename)
{
	ZeroMemory(&data1, sizeof(LayerData));
	lines.clear();
	numbLayers = 0;
	//data1.cellHeight = data1.cellWidth = data1.height = data1.width = 0;
	//data1.layers.resize(0);
	std::fstream file(filename);

	if (file.good())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::string str = line.substr(0, 5);
			if (str == "layer")
				numbLayers++;
			lines.push_back(line);
		}
		file.close();
		Parse();
	}
}

MapFileParser::MapFileParser(const std::string& filename)
{
	
	Load(filename);
	
}



LayerData MapFileParser::operator()(const std::string & filename)
{
	Load(filename);
	lines.clear();
	return data1;
}

LayerData MapFileParser::GetData() const
{
	return data1;
}


MapFileParser::~MapFileParser()
{
}
