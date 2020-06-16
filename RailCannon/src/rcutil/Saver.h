#pragma once

#include <string>
#include <vector>

class Saver
{
private:
	std::vector<int> m_WidthList;
	int m_DefaultIndex;
	bool m_DefaultFullscreen;

	int m_SavedIndex;
	int m_SavedWidth;
	int m_SavedHeight;
	int m_Fullscreen;

	std::string m_Path;

public:
	Saver(const std::string path);
	~Saver();

	void Load();
	void Save(int index, bool fullscreen);

	inline int GetWidthBasedOnIndex(const int index) const { return m_WidthList[index]; }
	inline int GetHeightBasedOnIndex(const int index) const { return GetHeightFromWidth(m_WidthList[index]); }

	inline int GetSavedIndex() const { return m_SavedIndex; }
	inline int GetSavedWidth() const { return m_SavedWidth; }
	inline int GetSavedHeight() const { return m_SavedHeight; }
	inline bool GetFullscreen() const { return m_Fullscreen; }

private:
	inline int GetHeightFromWidth(int width) const { return (width * 9) / 16; }
};