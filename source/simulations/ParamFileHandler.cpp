#include "ParamFileHandler.h"
#include <fstream>
#include <sstream>

using namespace std;

namespace SIMULATION
{

void ParamFileHandler::readParamFile(const char * file) {
	
	ifstream ifs(file);
	char buffer[4096];
	
	while (!ifs.eof()) {

		ifs.getline(buffer,sizeof(buffer));
	
		istringstream iss(buffer); 
		string name;
		string value;
		string tmp;
		int n = 0;

		while (!iss.eof()) {
			if (n==0) {
				iss >> name;
			} else {
				iss >> tmp;
				if (n>1) {
					value+=" ";
				}
				value+=tmp;
			}
			n++;
		}

		if (name.empty() || value.empty()) {
			//exception??
			continue;
		}
		addParamString(name, value);
	}
}

}