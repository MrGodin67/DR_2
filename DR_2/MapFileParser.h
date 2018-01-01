#pragma once
#include <iostream>


struct LayerData
{
	wchar_t tileSetFilename[MAX_PATH];
	wchar_t entitySetFilename[MAX_PATH];
	int width;
	int height;
	int cellWidth;
	int cellHeight;
	int drawWidth;
	int drawHeight;
	std::vector<vint> layers;
};
class MapFileParser
{
	vstring lines;
	LayerData data1;
	int numbLayers = 0;
	void Parse();
	void Load(const std::string& filename);
public:
	MapFileParser() = default;
	MapFileParser(const std::string& filename);
	LayerData operator ()(const std::string& filename);
	LayerData GetData()const;
	~MapFileParser();
};

