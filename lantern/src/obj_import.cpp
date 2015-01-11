#include <fstream>
#include <sstream>
#include <stdexcept>
#include "obj_import.h"

using namespace lantern;

void obj_reader::read(std::string const& path)
{
	// Open file
	//
	std::ifstream file_stream{path};
	if (!file_stream.is_open())
	{
		throw std::runtime_error("Could not open file: " + path);
	}

	on_reading_started();

	// Read line-by-line
	//
	std::string line;
	parse_face_function parse_face{nullptr};

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
			float x, y, z;
			line_stream >> x >> y >> z;
			on_vertex_def(x, y, z);
		}
		else if (definition_type == "vn")
		{
			float x, y, z;
			line_stream >> x >> y >> z;
			on_normal_def(x, y, z);
		}
		else if (definition_type == "vt")
		{
			float x, y;
			line_stream >> x >> y;
			on_texcoord_def(x, y);
		}
		else if (definition_type == "f")
		{
			if (parse_face == nullptr)
			{
				parse_face = get_parse_function(line);
			}

			on_face_def_started();

			(this->*parse_face)(line_stream);

			on_face_def_ended();
		}

		file_stream >> std::ws;
	}

	on_reading_ended();
}

obj_reader::parse_face_function obj_reader::get_parse_function(std::string face_definition)
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
		return &obj_reader::parse_face_vertex;
	}

	// Otherwise, it's '/'

	stream >> ui;
	if (stream.fail())
	{
		// Definition us "v//n"
		return &obj_reader::parse_face_vertex_normal;
	}

	stream >> c;
	if (c == '/')
	{
		// Definition is "v/uv/n"
		return &obj_reader::parse_face_vertex_texcoord_normal;
	}

	// Definition is "v/uv"
	return &obj_reader::parse_face_vertex_texcoord;
}

void obj_reader::parse_face_vertex(std::istringstream& stream)
{
	unsigned int index0, index1, index2;
	stream >> index0 >> index1 >> index2;

	on_face_pos_def(index0 - 1, index1 - 1, index2 - 1);
}

void obj_reader::parse_face_vertex_normal(std::istringstream& stream)
{
	unsigned int vertex_index0, vertex_index1, vertex_index2;
	unsigned int normal_index0, normal_index1, normal_index2;

	stream >> vertex_index0;
	stream.ignore(2);
	stream >> normal_index0;

	stream >> vertex_index1;
	stream.ignore(2);
	stream >> normal_index1;

	stream >> vertex_index2;
	stream.ignore(2);
	stream >> normal_index2;

	on_face_pos_def(vertex_index0 - 1, vertex_index1 - 1, vertex_index2 - 1);
	on_face_normal_def(normal_index0 - 1, normal_index1 - 1, normal_index2 - 1);
}

void obj_reader::parse_face_vertex_texcoord(std::istringstream& stream)
{
	unsigned int vertex_index0, vertex_index1, vertex_index2;
	unsigned int texcoord_index0, texcoord_index1, texcoord_index2;

	stream >> vertex_index0;
	stream.ignore();
	stream >> texcoord_index0;

	stream >> vertex_index1;
	stream.ignore();
	stream >> texcoord_index1;

	stream >> vertex_index2;
	stream.ignore();
	stream >> texcoord_index2;

	on_face_pos_def(vertex_index0 - 1, vertex_index1 - 1, vertex_index2 - 1);
	on_face_texcoord_def(texcoord_index0 - 1, texcoord_index1 - 1, texcoord_index2 - 1);
}

void obj_reader::parse_face_vertex_texcoord_normal(std::istringstream& stream)
{
	unsigned int vertex_index0, vertex_index1, vertex_index2;
	unsigned int normal_index0, normal_index1, normal_index2;
	unsigned int texcoord_index0, texcoord_index1, texcoord_index2;

	stream >> vertex_index0;
	stream.ignore();
	stream >> texcoord_index0;
	stream.ignore();
	stream >> normal_index0;

	stream >> vertex_index1;
	stream.ignore();
	stream >> texcoord_index1;
	stream.ignore();
	stream >> normal_index1;

	stream >> vertex_index2;
	stream.ignore();
	stream >> texcoord_index2;
	stream.ignore();
	stream >> normal_index2;

	on_face_pos_def(vertex_index0 - 1, vertex_index1 - 1, vertex_index2 - 1);
	on_face_texcoord_def(texcoord_index0 - 1, texcoord_index1 - 1, texcoord_index2 - 1);
	on_face_normal_def(normal_index0 - 1, normal_index1 - 1, normal_index2 - 1);
}

void obj_reader::on_reading_started()
{

}

void obj_reader::on_reading_ended()
{

}

void obj_reader::on_vertex_def(float const x, float const y, float const z)
{

}

void obj_reader::on_texcoord_def(float const x, float const y)
{

}

void obj_reader::on_normal_def(float const x, float const y, float const z)
{

}

void obj_reader::on_face_def_started()
{

}

void obj_reader::on_face_def_ended()
{

}

void obj_reader::on_face_pos_def(unsigned int vertex_index0, unsigned int vertex_index1, unsigned int vertex_index2)
{

}

void obj_reader::on_face_texcoord_def(unsigned int texcoord_index0, unsigned int texcoord_index1, unsigned int texcoord_index2)
{

}

void obj_reader::on_face_normal_def(unsigned int normal_index0, unsigned int normal_index1, unsigned int normal_index2)
{

}

// obj_mesh_importer
//

obj_mesh_importer::obj_mesh_importer(bool const read_texcoords, bool const read_normals)
	: m_read_texcoords(read_texcoords), m_read_normals(read_normals)
{

}

mesh obj_mesh_importer::get_mesh() const
{
	mesh m{m_vertices, m_indices};

	if (m_read_texcoords)
	{
		mesh_attribute_info<vector2f> texcoords_info{TEXCOORD_ATTR_ID, m_texcoords, m_texcoords_indices};
		m.get_vector2f_attributes().push_back(texcoords_info);
	};

	if (m_read_normals)
	{
		mesh_attribute_info<vector3> normals_info{NORMAL_ATTR_ID, m_normals, m_normals_indices};
		m.get_vector3_attributes().push_back(normals_info);
	}

	return m;
}

void obj_mesh_importer::on_reading_started()
{
	m_vertices.clear();
	m_indices.clear();

	m_texcoords.clear();
	m_texcoords_indices.clear();

	m_normals.clear();
	m_normals_indices.clear();
}

void obj_mesh_importer::on_vertex_def(float const x, float const y, float const z)
{
	m_vertices.push_back(vector3{x, y, z});
}

void obj_mesh_importer::on_texcoord_def(float const x, float const y)
{
	if (m_read_texcoords)
	{
		m_texcoords.push_back(vector2f{x, y});
	}
}

void obj_mesh_importer::on_normal_def(float const x, float const y, float const z)
{
	if (m_read_normals)
	{
		m_normals.push_back(vector3{x, y, z});
	}
}

void obj_mesh_importer::on_face_pos_def(unsigned int vertex_index0, unsigned int vertex_index1, unsigned int vertex_index2)
{
	m_indices.push_back(vertex_index0);
	m_indices.push_back(vertex_index1);
	m_indices.push_back(vertex_index2);
}

void obj_mesh_importer::on_face_texcoord_def(unsigned int texcoord_index0, unsigned int texcoord_index1, unsigned int texcoord_index2)
{
	if (m_read_texcoords)
	{
		m_texcoords_indices.push_back(texcoord_index0);
		m_texcoords_indices.push_back(texcoord_index1);
		m_texcoords_indices.push_back(texcoord_index2);
	}
}

void obj_mesh_importer::on_face_normal_def(unsigned int normal_index0, unsigned int normal_index1, unsigned int normal_index2)
{
	if (m_read_normals)
	{
		m_normals_indices.push_back(normal_index0);
		m_normals_indices.push_back(normal_index1);
		m_normals_indices.push_back(normal_index2);
	}
}

// Helper functions
//

mesh lantern::load_mesh_from_obj(std::string const& path, bool const read_texcoords, bool const read_normals)
{
	obj_mesh_importer importer{read_texcoords, read_normals};
	importer.read(path);

	return importer.get_mesh();
}
