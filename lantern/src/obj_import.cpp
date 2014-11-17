#include <fstream>
#include <sstream>
#include <stdexcept>
#include "obj_import.h"

using namespace lantern;

typedef void (*parse_face_function)(std::istringstream&, face&);

// Decrement all face indices
// Need this because .obj format counts from 1, but we do it from 0
//
static void face_decrement_values(face& f)
{
	--f.vertex_index0;
	--f.vertex_index1;
	--f.vertex_index2;

	--f.uv_index0;
	--f.uv_index1;
	--f.uv_index2;

	--f.normal_index0;
	--f.normal_index1;
	--f.normal_index2;
}

static void parse_face_vertex(std::istringstream& stream, face& f)
{
	stream >> f.vertex_index0 >> f.vertex_index1 >> f.vertex_index2;
	face_decrement_values(f);
}

static void parse_face_vertex_uv(std::istringstream& stream, face& f)
{
	stream >> f.vertex_index0;
	stream.ignore();
	stream >> f.uv_index0;

	stream >> f.vertex_index1;
	stream.ignore();
	stream >> f.uv_index1;

	stream >> f.vertex_index2;
	stream.ignore();
	stream >> f.uv_index2;

	face_decrement_values(f);
}

static void parse_face_vertex_uv_normal(std::istringstream& stream, face& f)
{
	stream >> f.vertex_index0;
	stream.ignore();
	stream >> f.uv_index0;
	stream.ignore();
	stream >> f.normal_index0;

	stream >> f.vertex_index1;
	stream.ignore();
	stream >> f.uv_index1;
	stream.ignore();
	stream >> f.normal_index1;

	stream >> f.vertex_index2;
	stream.ignore();
	stream >> f.uv_index2;
	stream.ignore();
	stream >> f.normal_index2;

	face_decrement_values(f);
}

static void parse_face_vertex_normal(std::istringstream& stream, face& f)
{
	stream >> f.vertex_index0;
	stream.ignore(2);
	stream >> f.normal_index0;

	stream >> f.vertex_index1;
	stream.ignore(2);
	stream >> f.normal_index1;

	stream >> f.vertex_index2;
	stream.ignore(2);
	stream >> f.normal_index2;

	face_decrement_values(f);
}

static parse_face_function get_parse_function(std::string face_definition)
{
	std::istringstream stream{face_definition};

	char c;
	unsigned int ui;

	// Skip "f"
	stream >> c;

	// First number is vertex index anyway, skip it together with preceding whitespaces
	stream >> std::skipws >> ui >> std::noskipws;

	stream >> c;
	if (c == ' ')
	{
		// Whitespace immediately after vertex index => no uv, no normals
		return parse_face_vertex;
	}

	// Otherwise, it's '/'

	stream >> ui;
	if (stream.fail())
	{
		// Definition us "v//n"
		return parse_face_vertex_normal;
}

	stream >> c;
	if (c == '/')
	{
		// Definition is "v/uv/n"
		return parse_face_vertex_uv_normal;
	}

	// Definition is "v/uv"
	return parse_face_vertex_uv;
}

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
	std::vector<vector3> normals;
	std::vector<vector3> uvs;
	std::vector<face> faces;

	parse_face_function parse_face{nullptr};

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
		else if (definition_type == "vn")
		{
			vector3 normal;
			line_stream >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (definition_type == "vt")
		{
			vector3 uv;
			line_stream >> uv.x >> uv.y;
			uvs.push_back(uv);
		}
		else if (definition_type == "f")
		{
			if (parse_face == nullptr)
			{
				parse_face = get_parse_function(line);
			}

			face f;
			parse_face(line_stream, f);
			faces.push_back(f);
		}

		file_stream >> std::ws;
	}

	return std::shared_ptr<mesh>{new mesh{vertices, uvs, normals, faces}};
}
