#include "common.h"
#include "obj_import.h"

using namespace lantern;

static void assert_obj_vertices(mesh const& mesh)
{
	// Test positions
	//

	std::vector<vector3> const correct_vertices
		{ vector3{0.0f, 0.0f, 0.0f},
		  vector3{0.0f, 0.0f, 1.0f},
		  vector3{0.0f, 1.0f, 0.0f},
		  vector3(0.0f, 1.0f, 1.0f),
		  vector3{1.0f, 0.0f, 0.0f},
		  vector3{1.0f, 0.0f, 1.0f},
		  vector3{1.0f, 1.0f, 0.0f},
		  vector3{1.0f, 1.0f, 1.0f} };

	std::vector<vector3> const& vertices{mesh.get_vertices()};

	ASSERT_EQ(vertices.size(), 8);
	for (size_t i{0}; i < 8; ++i)
	{
		assert_vectors3_near(correct_vertices[i], vertices[i]);
	}

	// Test indices
	//

	std::vector<unsigned int> const& indices{mesh.get_indices()};

	std::vector<unsigned int> const correct_indices
		{ 0, 6, 4,
		  0, 2, 6,
		  0, 3, 2,
		  0, 1, 3,
		  2, 7, 6,
		  2, 3, 7,
		  4, 6, 7,
		  4, 7, 5,
		  0, 4, 5,
		  0, 5, 1,
		  1, 5, 7,
		  1, 7, 3 };

	ASSERT_EQ(indices.size(), 36);
	for (size_t i{0}; i < 36; ++i)
	{
		ASSERT_EQ(correct_indices[i], indices[i]);
	}
}

static void assert_obj_texcoords(mesh const& mesh)
{
	mesh_attribute_info<vector2f> const* texcoords_info{nullptr};
	for (size_t i{0}; i < mesh.get_vector2_attributes_storage().size(); ++i)
	{
		mesh_attribute_info<vector2f> const& attribute_info = mesh.get_vector2_attributes_storage()[i];
		if (attribute_info.get_id() == "texcoords")
		{
			texcoords_info = &attribute_info;
		}
	}
	ASSERT_NE(texcoords_info, nullptr);

	std::vector<vector2f> const correct_texcoords
		{ vector2f{0.0f, 0.0f},
		  vector2f{1.0f, 0.0f},
		  vector2f{1.0f, 1.0f},
		  vector2f(0.0f, 1.0f) };

	ASSERT_EQ(texcoords_info->get_data().size(), 4);
	for (size_t i{0}; i < 4; ++i)
	{
		assert_vectors2_near(correct_texcoords[i], texcoords_info->get_data()[i]);
	}

	std::vector<unsigned int> const correct_indices
		{ 0, 2, 1,
		  0, 3, 2,
		  0, 2, 3,
		  0, 1, 2,
		  0, 2, 3,
		  0, 1, 2,
		  1, 2, 3,
		  1, 3, 0,
		  0, 1, 2,
		  0, 2, 3,
		  0, 1, 2,
		  0, 2, 3 };

	ASSERT_EQ(texcoords_info->get_indices().size(), 36);
	for (size_t i{0}; i < 36; ++i)
	{
		ASSERT_EQ(correct_indices[i], texcoords_info->get_indices()[i]);
	}
}

void assert_obj_normals(mesh const& mesh)
{
	mesh_attribute_info<vector3> const* normals_info{nullptr};
	for (size_t i{0}; i < mesh.get_vector3_attributes_storage().size(); ++i)
	{
		mesh_attribute_info<vector3> const& attribute_info = mesh.get_vector3_attributes_storage()[i];
		if (attribute_info.get_id() == "normals")
		{
			normals_info = &attribute_info;
		}
	}
	ASSERT_NE(normals_info, nullptr);

	std::vector<vector3> const correct_normals
		{ vector3{0.0f, 0.0f, 1.0f},
		  vector3{0.0f, 0.0f, -1.0f},
		  vector3{0.0f, 1.0f, 0.0f},
		  vector3{0.0f, -1.0f, 0.0f},
		  vector3{1.0f, 0.0f, 0.0f},
		  vector3{-1.0f, 0.0f, 0.0f} };

	ASSERT_EQ(normals_info->get_data().size(), 6);
	for (size_t i{0}; i < 6; ++i)
	{
		assert_vectors3_near(correct_normals[i], normals_info->get_data()[i]);
	}

	std::vector<unsigned int> const correct_indices
		{ 1, 1, 1,
		  1, 1, 1,
		  5, 5, 5,
		  5, 5, 5,
		  2, 2, 2,
		  2, 2, 2,
		  4, 4, 4,
		  4, 4, 4,
		  3, 3, 3,
		  3, 3, 3,
		  0, 0, 0,
		  0, 0, 0 };
	ASSERT_EQ(normals_info->get_indices().size(), 36);
	for (size_t i{0}; i < 36; ++i)
	{
		ASSERT_EQ(correct_indices[i], normals_info->get_indices()[i]);
	}
}

TEST(obj_import, import_obj_mesh)
{
	mesh const& unit_cube_mesh_pos{load_mesh_from_obj("resources/unit_cube_pos.obj", false, false)};
	assert_obj_vertices(unit_cube_mesh_pos);

	mesh const& unit_cube_mesh_pos_texcoord{load_mesh_from_obj("resources/unit_cube_pos_texcoord.obj", true, false)};
	assert_obj_vertices(unit_cube_mesh_pos_texcoord);
	assert_obj_texcoords(unit_cube_mesh_pos_texcoord);

	mesh const& unit_cube_mesh_pos_normal{load_mesh_from_obj("resources/unit_cube_pos_normal.obj", false, true)};
	assert_obj_vertices(unit_cube_mesh_pos_normal);
	assert_obj_normals(unit_cube_mesh_pos_normal);

	mesh const& unit_cube_mesh_pos_texcoord_normal{load_mesh_from_obj("resources/unit_cube_pos_texcoord_normal.obj", true, true)};
	assert_obj_vertices(unit_cube_mesh_pos_texcoord_normal);
	assert_obj_texcoords(unit_cube_mesh_pos_texcoord_normal);
	assert_obj_normals(unit_cube_mesh_pos_texcoord_normal);
}
