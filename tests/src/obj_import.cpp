#include "common.h"
#include "obj_import.h"

using namespace lantern;

TEST(obj_import, import_obj_mesh)
{
	std::shared_ptr<mesh const> unit_cube_mesh{import_obj_mesh("resources/unit_cube.obj")};
	std::vector<vector3> const& vertices{unit_cube_mesh->get_vertices()};
	std::vector<face> const& faces{unit_cube_mesh->get_faces()};
	ASSERT_EQ(vertices.size(), 8);
	ASSERT_EQ(faces.size(), 12);
	ASSERT_FLOATS_NEAR(vertices[0].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[0].y, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[0].z, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[1].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[1].y, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[1].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[2].x, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[2].y, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[2].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[3].x, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[3].y, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[3].z, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[4].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[4].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[4].z, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[5].x, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[5].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[5].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[6].x, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[6].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[6].z, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[7].x, -1.0f);
	ASSERT_FLOATS_NEAR(vertices[7].y, 1.0f);
	ASSERT_FLOATS_NEAR(vertices[7].z, -1.0f);
	ASSERT_EQ(faces[0].index1, 0);
	ASSERT_EQ(faces[0].index2, 1);
	ASSERT_EQ(faces[0].index3, 3);
	ASSERT_EQ(faces[1].index1, 4);
	ASSERT_EQ(faces[1].index2, 7);
	ASSERT_EQ(faces[1].index3, 5);
	ASSERT_EQ(faces[2].index1, 0);
	ASSERT_EQ(faces[2].index2, 4);
	ASSERT_EQ(faces[2].index3, 1);
	ASSERT_EQ(faces[3].index1, 1);
	ASSERT_EQ(faces[3].index2, 5);
	ASSERT_EQ(faces[3].index3, 2);
	ASSERT_EQ(faces[4].index1, 2);
	ASSERT_EQ(faces[4].index2, 6);
	ASSERT_EQ(faces[4].index3, 3);
	ASSERT_EQ(faces[5].index1, 4);
	ASSERT_EQ(faces[5].index2, 0);
	ASSERT_EQ(faces[5].index3, 7);
	ASSERT_EQ(faces[6].index1, 1);
	ASSERT_EQ(faces[6].index2, 2);
	ASSERT_EQ(faces[6].index3, 3);
	ASSERT_EQ(faces[7].index1, 7);
	ASSERT_EQ(faces[7].index2, 6);
	ASSERT_EQ(faces[7].index3, 5);
	ASSERT_EQ(faces[8].index1, 4);
	ASSERT_EQ(faces[8].index2, 5);
	ASSERT_EQ(faces[8].index3, 1);
	ASSERT_EQ(faces[9].index1, 5);
	ASSERT_EQ(faces[9].index2, 6);
	ASSERT_EQ(faces[9].index3, 2);
	ASSERT_EQ(faces[10].index1, 6);
	ASSERT_EQ(faces[10].index2, 7);
	ASSERT_EQ(faces[10].index3, 3);
	ASSERT_EQ(faces[11].index1, 0);
	ASSERT_EQ(faces[11].index2, 3);
	ASSERT_EQ(faces[11].index3, 7);
}
