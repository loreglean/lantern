#include <fstream>
#include <sstream>
#include <stdexcept>
#include "obj_import.h"

using namespace lantern;

std::shared_ptr<mesh const> lantern::import_obj_mesh(std::string path)
{
	// Open file
	//
	std::ifstream file_stream{path};
	if (!file_stream.is_open())
	{
		throw std::runtime_error("Could not open file: " + path);
	}

	std::vector<vector3> vertices;
	std::vector<face> faces;

	// Read line-by-line
	//
	std::string line;
	file_stream >> std::ws;
	while(std::getline(file_stream, line))
	{
		// Skip comments
		//
		if (line[0] == '#')
		{
			continue;
		}

		// Get line definition type
		//

		std::istringstream line_stream{line};

		std::string definition_type;
		line_stream >> definition_type;

		// Process each definition
		//

		if (definition_type == "v")
		{
			vector3 vertex;
			line_stream >> vertex.x >> vertex.y >> vertex.z;
			vertices.push_back(vertex);
		}
		else if (definition_type == "f")
		{
			face f;
			line_stream >> f.index1 >> f.index2 >> f.index3;

			// Decrement faces indices, because .obj format starts counting them from 1
			//
			--f.index1;
			--f.index2;
			--f.index3;

			faces.push_back(f);
		}

		file_stream >> std::ws;
	}

	return std::shared_ptr<mesh>{new mesh{vertices, faces}};
}
