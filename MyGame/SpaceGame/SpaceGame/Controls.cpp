#include "Controls.h"

IND_Key Controls::getAdvance()
{
	return keyMap["advance"];
}

IND_Key Controls::getRotateLeft()
{
	return keyMap["rotateLeft"];
}

IND_Key Controls::getRotateRight()
{
	return keyMap["rotateRight"];
}

IND_Key Controls::getQuickSave()
{
	return keyMap["quickSave"];
}

IND_Key Controls::getQuickLoad()
{
	return keyMap["quickLoad"];
}

void Controls::loadSettings()
{
	map<const char*,const char*> stringKeys;
	ifstream indKeys, customKeys;
	indKeys.open("Settings/IndieLib_Keys.txt", ios::in);
	customKeys.open("Settings/Custom_Keys.txt", ios::in);
	if (indKeys.is_open() && customKeys.is_open())
	{
		string line = "";
		while (std::getline(customKeys, line))
		{
			if (line[0] != '#')
			{
				char* control = new char[30];
				strncpy(control, line.substr(line.find_first_of("[") + 1, line.find_first_of("]") - line.find_first_of("[") - 1).c_str(), 29);
				keyMap[control] = IND_0;
				char* key = new char[30];
				strncpy(key, line.substr(line.find_first_of(":") + 1).c_str(), 29);
				stringKeys[control] = key;
			}
		}
		int count = 0;
		while (std::getline(indKeys, line))
		{
			char* indKey = new char[40];
			strncpy(indKey, line.substr(line.find_first_of("!") + 2).c_str(), 39);
			for (std::map<const char*, const char*>::iterator it = stringKeys.begin(); it != stringKeys.end(); ++it)
			{
				if (!strcmpi(indKey, it->second))
				{
					keyMap[it->first] = count;
				}
			}
			delete[] indKey;
			std::getline(indKeys, line);
			count++;
		}
	}
	indKeys.close();
	customKeys.close();
}
