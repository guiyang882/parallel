#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>
#include "sparserepresentation.hpp"

using namespace std;

bool readSingleDictFile(string filepath, vector<vector<double>> &dict) {
	if(access(filepath.c_str(), F_OK) != 0) return false;
	ifstream fin(filepath.c_str());
	string line = "";
	char delims[] = ",";
	while(getline(fin, line)) {
		vector<double> tmp;
		char *token = strtok(const_cast<char*>(line.c_str()), delims);
		while(token != NULL) {
			tmp.push_back(atof(token));
			token = strtok(NULL, delims);
		}
		delete token; token = NULL;
		dict.push_back(tmp);
	}
	fin.close();
	return true;
}

bool loadDicts(string filepath, vector<vector<vector<double>>>& dict) {
	if(access(filepath.c_str(), F_OK) != 0) return false;
	ifstream fin(filepath);
	string path = "";
	while(getline(fin, path)) {
		vector<vector<double>> tmpDict;
		bool flag = readSingleDictFile(path, tmpDict);
		if(flag) {
			dict.push_back(tmpDict);
		} else {
			fin.close();
			return false;
		}
	}
	fin.close();
	return true;
}

int main() {
	string dictlist = "/Volumes/Ubuntu/RetrieveImageDic/dict.list";
	vector<vector<vector<double>>> dict;
	bool flag = loadDicts(dictlist, dict);
	if(flag) {
		LSparseRepresentation<double> obj(dict);
		string imgFeaturePath = "/Volumes/Ubuntu/RetrieveImageDic/imagefeater/1.csv";
		vector<vector<double>> img; vector<int> srres;
		if(readSingleDictFile(imgFeaturePath, img)) {
			if(obj.SRClassify(img, 1.0, 9, srres)) {
				for(auto resd:srres) {
					cout << resd << endl;
				}
			}
		}
	} else {
		cout << "Load Dicts Error !" << endl;
	}
	
	return 0;
}