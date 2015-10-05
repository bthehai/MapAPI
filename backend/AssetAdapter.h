#pragma once
#include <string>
#include <iostream>
#include <fstream>

class AssetAdapter {

public:
	static AssetAdapter* pG;

protected:
	virtual void* openFileAsStream(std::string file) = 0;

public:
	static void init(AssetAdapter *pAdapter) {
		pG = pAdapter;
	}

	static void* readFileAsStream(std::string file) {
		return pG->openFileAsStream(file);
	}

	static std::string readTextFile(std::string file) {
		std::string temp = "";
		std::string line = "";
		std::ifstream infile;
		infile.open(file.c_str());
		while (!infile.eof()) {
			getline(infile, line);
			temp += line;
		}
		infile.close();
		return temp;		
	}
};