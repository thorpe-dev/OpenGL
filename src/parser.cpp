#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <fstream>
#include <istream>
#include "cgRender.h"

namespace parser
{
	using namespace std;
	using namespace boost;

	vtk_file* parse_vtk_file(string s)
	{
		int section = 0;
		vtk_file* input = (vtk_file*)malloc(sizeof(struct vtk_file));
		vector<float> points;
		vector<int> polygons;

		char_separator<char> sep(" ,\n");

		tokenizer<char_separator<char> > tok(s,sep);

		for(tokenizer<char_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)		
		{
			while ((*beg) != "POINTS")
			{
				beg++;
			}


			section++;
			beg++;
			//cout << *beg << endl;
			int point_count = lexical_cast<int>(*beg++);
			cout << point_count << endl;
			beg++;
			while(*beg != "POLYGONS")
			{
				points.push_back(lexical_cast<float>(*beg++));
			}
			cout << points.size() << endl;

			beg++;

			int polygon_no = lexical_cast<int>(*beg);
			beg++;
			int polygon_size = lexical_cast<int>(*beg);
			beg++;

			cout << polygon_no << " no polygons, " << polygon_size << " polygon_size" << endl;

			while (*beg != "POINT_DATA")
			{
				polygons.push_back(lexical_cast<int>(*beg++));
			}

			cout << polygons.size() << endl;

			


			//cout << *beg << endl;
		}
		return NULL;
	}

}

int main (int argc, char** argv)
{
	using namespace std;
	// Check input arguments
	// and print a usage on failure
	if (argc != 3)
	{
		cout << "Usage: " << argv[0] << "and then a .vtk file, followed by 1 (meaning Gourand rendered "
			<< "or 2 (meaning texture mapped render)" << endl;
		exit(EXIT_FAILURE);
	}

	//int length;
	ifstream inputFile(argv[1]);


	// get length of file:
	inputFile.seekg (0, std::ios::end);
	int length = inputFile.tellg();
	inputFile.seekg (0, std::ios::beg);

	// allocate memory for the string
	char* buffer = new char[length];

	inputFile.read (buffer,length);
	inputFile.close();
	std::string vtk_input = buffer;

	vtk_file* file = parser::parse_vtk_file((string)buffer);
}
