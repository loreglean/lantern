#include "common.h"
#include "obj_import.h"

using namespace lantern;

static void assert_obj_vertices(std::shared_ptr<mesh const> mesh)
{
	std::vector<vector3> const& vertices{mesh->get_vertices()};
	ASSERT_EQ(vertices.size(), 8);
	ASSERT_FLOATS_NEAR(vertices[0].x, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[0].y, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[0].z, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[1].x, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[1].y, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[1].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[2].x, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[2].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[2].z, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[3].x, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[3].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[3].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[4].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[4].y, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[4].z, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[5].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[5].y, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[5].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[6].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[6].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[6].z, 0.0f);
	ASSERT_FLOATS_NEAR(vertices[7].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[7].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[7].z, 1.0f);
}

static void assert_obj_faces_vertices_indices(std::shared_ptr<mesh const> mesh)
{
	std::vector<face> const& faces{mesh->get_faces()};
	ASSERT_EQ(faces.size(), 12);
	ASSERT_EQ(faces[0].vertex_index0, 0);
	ASSERT_EQ(faces[0].vertex_index1, 6);
	ASSERT_EQ(faces[0].vertex_index2, 4);
	ASSERT_EQ(faces[1].vertex_index0, 0);
	ASSERT_EQ(faces[1].vertex_index1, 2);
	ASSERT_EQ(faces[1].vertex_index2, 6);
	ASSERT_EQ(faces[2].vertex_index0, 0);
	ASSERT_EQ(faces[2].vertex_index1, 3);
	ASSERT_EQ(faces[2].vertex_index2, 2);
	ASSERT_EQ(faces[3].vertex_index0, 0);
	ASSERT_EQ(faces[3].vertex_index1, 1);
	ASSERT_EQ(faces[3].vertex_index2, 3);
	ASSERT_EQ(faces[4].vertex_index0, 2);
	ASSERT_EQ(faces[4].vertex_index1, 7);
	ASSERT_EQ(faces[4].vertex_index2, 6);
	ASSERT_EQ(faces[5].vertex_index0, 2);
	ASSERT_EQ(faces[5].vertex_index1, 3);
	ASSERT_EQ(faces[5].vertex_index2, 7);
	ASSERT_EQ(faces[6].vertex_index0, 4);
	ASSERT_EQ(faces[6].vertex_index1, 6);
	ASSERT_EQ(faces[6].vertex_index2, 7);
	ASSERT_EQ(faces[7].vertex_index0, 4);
	ASSERT_EQ(faces[7].vertex_index1, 7);
	ASSERT_EQ(faces[7].vertex_index2, 5);
	ASSERT_EQ(faces[8].vertex_index0, 0);
	ASSERT_EQ(faces[8].vertex_index1, 4);
	ASSERT_EQ(faces[8].vertex_index2, 5);
	ASSERT_EQ(faces[9].vertex_index0, 0);
	ASSERT_EQ(faces[9].vertex_index1, 5);
	ASSERT_EQ(faces[9].vertex_index2, 1);
	ASSERT_EQ(faces[10].vertex_index0, 1);
	ASSERT_EQ(faces[10].vertex_index1, 5);
	ASSERT_EQ(faces[10].vertex_index2, 7);
	ASSERT_EQ(faces[11].vertex_index0, 1);
	ASSERT_EQ(faces[11].vertex_index1, 7);
	ASSERT_EQ(faces[11].vertex_index2, 3);
}

static void assert_obj_uvs(std::shared_ptr<mesh const> mesh)
{
	std::vector<vector3> const& uvs{mesh->get_uvs()};
	ASSERT_EQ(uvs.size(), 4);
	ASSERT_FLOATS_NEAR(uvs[0].x, 0.0f);
	ASSERT_FLOATS_NEAR(uvs[0].y, 0.0f);
	ASSERT_FLOATS_NEAR(uvs[1].x, 1.0f);
	ASSERT_FLOATS_NEAR(uvs[1].y, 0.0f);
	ASSERT_FLOATS_NEAR(uvs[2].x, 1.0f);
	ASSERT_FLOATS_NEAR(uvs[2].y, 1.0f);
	ASSERT_FLOATS_NEAR(uvs[3].x, 0.0f);
	ASSERT_FLOATS_NEAR(uvs[3].y, 1.0f);
}

static void assert_obj_faces_uvs_indices(std::shared_ptr<mesh const> mesh)
{
	std::vector<face> const& faces{mesh->get_faces()};
	ASSERT_EQ(faces.size(), 12);
	ASSERT_EQ(faces[0].uv_index0, 0);
	ASSERT_EQ(faces[0].uv_index1, 2);
	ASSERT_EQ(faces[0].uv_index2, 1);
	ASSERT_EQ(faces[1].uv_index0, 0);
	ASSERT_EQ(faces[1].uv_index1, 3);
	ASSERT_EQ(faces[1].uv_index2, 2);
	ASSERT_EQ(faces[2].uv_index0, 0);
	ASSERT_EQ(faces[2].uv_index1, 2);
	ASSERT_EQ(faces[2].uv_index2, 3);
	ASSERT_EQ(faces[3].uv_index0, 0);
	ASSERT_EQ(faces[3].uv_index1, 1);
	ASSERT_EQ(faces[3].uv_index2, 2);
	ASSERT_EQ(faces[4].uv_index0, 0);
	ASSERT_EQ(faces[4].uv_index1, 2);
	ASSERT_EQ(faces[4].uv_index2, 3);
	ASSERT_EQ(faces[5].uv_index0, 0);
	ASSERT_EQ(faces[5].uv_index1, 1);
	ASSERT_EQ(faces[5].uv_index2, 2);
	ASSERT_EQ(faces[6].uv_index0, 1);
	ASSERT_EQ(faces[6].uv_index1, 2);
	ASSERT_EQ(faces[6].uv_index2, 3);
	ASSERT_EQ(faces[7].uv_index0, 1);
	ASSERT_EQ(faces[7].uv_index1, 3);
	ASSERT_EQ(faces[7].uv_index2, 0);
	ASSERT_EQ(faces[8].uv_index0, 0);
	ASSERT_EQ(faces[8].uv_index1, 1);
	ASSERT_EQ(faces[8].uv_index2, 2);
	ASSERT_EQ(faces[9].uv_index0, 0);
	ASSERT_EQ(faces[9].uv_index1, 2);
	ASSERT_EQ(faces[9].uv_index2, 3);
	ASSERT_EQ(faces[10].uv_index0, 0);
	ASSERT_EQ(faces[10].uv_index1, 1);
	ASSERT_EQ(faces[10].uv_index2, 2);
	ASSERT_EQ(faces[11].uv_index0, 0);
	ASSERT_EQ(faces[11].uv_index1, 2);
	ASSERT_EQ(faces[11].uv_index2, 3);
}

static void assert_obj_normals(std::shared_ptr<mesh const> mesh)
{
	std::vector<vector3> const& normals{mesh->get_normals()};
	ASSERT_EQ(normals.size(), 6);
	ASSERT_FLOATS_NEAR(normals[0].x, 0.0f);
	ASSERT_FLOATS_NEAR(normals[0].y, 0.0f);
	ASSERT_FLOATS_NEAR(normals[0].z, 1.0f);
	ASSERT_FLOATS_NEAR(normals[1].x, 0.0f);
	ASSERT_FLOATS_NEAR(normals[1].y, 0.0f);
	ASSERT_FLOATS_NEAR(normals[1].z, -1.0f);
	ASSERT_FLOATS_NEAR(normals[2].x, 0.0f);
	ASSERT_FLOATS_NEAR(normals[2].y, 1.0f);
	ASSERT_FLOATS_NEAR(normals[2].z, 0.0f);
	ASSERT_FLOATS_NEAR(normals[3].x, 0.0f);
	ASSERT_FLOATS_NEAR(normals[3].y, -1.0f);
	ASSERT_FLOATS_NEAR(normals[3].z, 0.0f);
	ASSERT_FLOATS_NEAR(normals[4].x, 1.0f);
	ASSERT_FLOATS_NEAR(normals[4].y, 0.0f);
	ASSERT_FLOATS_NEAR(normals[4].z, 0.0f);
	ASSERT_FLOATS_NEAR(normals[5].x, -1.0f);
	ASSERT_FLOATS_NEAR(normals[5].y, 0.0f);
	ASSERT_FLOATS_NEAR(normals[5].z, 0.0f);
}

static void assert_obj_faces_normals_indices(std::shared_ptr<mesh const> mesh)
{
	std::vector<face> const& faces{mesh->get_faces()};
	ASSERT_EQ(faces.size(), 12);
	ASSERT_EQ(faces[0].normal_index0, 1);
	ASSERT_EQ(faces[0].normal_index1, 1);
	ASSERT_EQ(faces[0].normal_index2, 1);
	ASSERT_EQ(faces[1].normal_index0, 1);
	ASSERT_EQ(faces[1].normal_index1, 1);
	ASSERT_EQ(faces[1].normal_index2, 1);
	ASSERT_EQ(faces[2].normal_index0, 5);
	ASSERT_EQ(faces[2].normal_index1, 5);
	ASSERT_EQ(faces[2].normal_index2, 5);
	ASSERT_EQ(faces[3].normal_index0, 5);
	ASSERT_EQ(faces[3].normal_index1, 5);
	ASSERT_EQ(faces[3].normal_index2, 5);
	ASSERT_EQ(faces[4].normal_index0, 2);
	ASSERT_EQ(faces[4].normal_index1, 2);
	ASSERT_EQ(faces[4].normal_index2, 2);
	ASSERT_EQ(faces[5].normal_index0, 2);
	ASSERT_EQ(faces[5].normal_index1, 2);
	ASSERT_EQ(faces[5].normal_index2, 2);
	ASSERT_EQ(faces[6].normal_index0, 4);
	ASSERT_EQ(faces[6].normal_index1, 4);
	ASSERT_EQ(faces[6].normal_index2, 4);
	ASSERT_EQ(faces[7].normal_index0, 4);
	ASSERT_EQ(faces[7].normal_index1, 4);
	ASSERT_EQ(faces[7].normal_index2, 4);
	ASSERT_EQ(faces[8].normal_index0, 3);
	ASSERT_EQ(faces[8].normal_index1, 3);
	ASSERT_EQ(faces[8].normal_index2, 3);
	ASSERT_EQ(faces[9].normal_index0, 3);
	ASSERT_EQ(faces[9].normal_index1, 3);
	ASSERT_EQ(faces[9].normal_index2, 3);
	ASSERT_EQ(faces[10].normal_index0, 0);
	ASSERT_EQ(faces[10].normal_index1, 0);
	ASSERT_EQ(faces[10].normal_index2, 0);
	ASSERT_EQ(faces[11].normal_index0, 0);
	ASSERT_EQ(faces[11].normal_index1, 0);
	ASSERT_EQ(faces[11].normal_index2, 0);
}

TEST(obj_import, import_obj_mesh)
{
	std::shared_ptr<mesh const> unit_cube_v_mesh{import_obj_mesh("resources/unit_cube_v.obj")};
	assert_obj_vertices(unit_cube_v_mesh);
	assert_obj_faces_vertices_indices(unit_cube_v_mesh);

	std::shared_ptr<mesh const> unit_cube_v_uv_mesh{import_obj_mesh("resources/unit_cube_v_uv.obj")};
	assert_obj_vertices(unit_cube_v_uv_mesh);
	assert_obj_faces_vertices_indices(unit_cube_v_uv_mesh);
	assert_obj_uvs(unit_cube_v_uv_mesh);
	assert_obj_faces_uvs_indices(unit_cube_v_uv_mesh);

	std::shared_ptr<mesh const> unit_cube_v_uv_normal_mesh{import_obj_mesh("resources/unit_cube_v_uv_normal.obj")};
	assert_obj_vertices(unit_cube_v_uv_normal_mesh);
	assert_obj_faces_vertices_indices(unit_cube_v_uv_normal_mesh);
	assert_obj_uvs(unit_cube_v_uv_normal_mesh);
	assert_obj_faces_uvs_indices(unit_cube_v_uv_normal_mesh);
	assert_obj_normals(unit_cube_v_uv_normal_mesh);
	assert_obj_faces_normals_indices(unit_cube_v_uv_normal_mesh);

	std::shared_ptr<mesh const> unit_cube_v_normal_mesh{import_obj_mesh("resources/unit_cube_v_normal.obj")};
	assert_obj_vertices(unit_cube_v_normal_mesh);
	assert_obj_faces_vertices_indices(unit_cube_v_normal_mesh);
	assert_obj_normals(unit_cube_v_normal_mesh);
	assert_obj_faces_normals_indices(unit_cube_v_normal_mesh);
}
