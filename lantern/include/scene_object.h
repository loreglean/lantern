#ifndef LANTERN_SCENE_OBJECT
#define LANTERN_SCENE_OBJECT

#include <memory>
#include "mesh.h"

namespace lantern
{
	class scene_object final
	{
	public:
		scene_object(std::shared_ptr<mesh const> mesh);

		std::shared_ptr<mesh const> get_mesh() const;
		vector3 get_world_position() const;
		vector3 get_world_rotation() const;
		vector3& get_world_rotation();

		void set_world_position(vector3 const& position);
		void set_world_rotation(vector3 const& rotation);

	private:
		std::shared_ptr<mesh const> m_mesh;
		vector3 m_world_position;
		vector3 m_world_rotation;
	};
}

#endif
