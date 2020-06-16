#include "Saver.h"

#include <iostream>
#include <fstream>

#include "nlohmann/json.hpp"

Saver::Saver(const std::string path)
	:m_WidthList{ 1280, 1366, 1600, 1920 }, m_DefaultIndex(2), m_DefaultFullscreen(false), m_Path(path)
{
	m_SavedIndex = m_DefaultIndex;
	m_Fullscreen = m_DefaultFullscreen;
	m_SavedWidth = m_WidthList[m_SavedIndex];
	m_SavedHeight = GetHeightFromWidth(m_SavedWidth);
}

Saver::~Saver()
{
}

void Saver::Load()
{
	std::ifstream reader(m_Path);

	if (reader.is_open()) // File found
	{
		nlohmann::json jsn;

		reader >> jsn;
		m_SavedIndex = jsn["index"];
		m_Fullscreen = jsn["fullscreen"];

		reader.close();

		if (m_SavedIndex > m_WidthList.size() - 1)
			m_SavedIndex = m_WidthList.size() - 1;
		else if (m_SavedIndex < 0)
			m_SavedIndex = 0;

		m_SavedWidth = m_WidthList[m_SavedIndex];
		m_SavedHeight = GetHeightFromWidth(m_SavedWidth);

		std::cout << "[Saver Notification]: Save file loaded successfully." << std::endl;
	}
	else // File not found
	{
		std::cout << "[Saver Warning]: No saved settings found, saving defaults." << std::endl;
		Save(m_DefaultIndex, m_DefaultFullscreen);
	}
}

void Saver::Save(int index, bool fullscreen)
{
	std::ofstream writer(m_Path);
	if (writer.is_open())
	{
		nlohmann::json jsn;

		jsn["index"] = index;
		jsn["fullscreen"] = fullscreen;

		writer << jsn << std::endl;
		writer.close();
		std::cout << "[Saver Notification]: Save file written successfully." << std::endl;

		m_SavedIndex = index;
		m_Fullscreen = fullscreen;
		m_SavedWidth = m_WidthList[index];
		m_SavedHeight = GetHeightFromWidth(m_SavedWidth);
	}
	else
	{
		std::cout << "[Saver Error]: Permission denied to create save file." << std::endl;
	}
}
