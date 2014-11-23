#ifndef LANTERN_PIPELINE_H
#define LANTERN_PIPELINE_H

#include "shader.h"
#include "mesh.h"
#include "bitmap_painter.h"

namespace lantern
{
	/** Class that holds information abount binded attribute: it's info and bind point.
	*/
	template<typename T>
	class binded_mesh_attribute_info final
	{
	public:
		mesh_attribute_info<T> const* info;
		T* bind_point;
	};

	/** Class representing rendering pipeline.
	*	It can be treated as object that connects mesh data and shader to produce image.
	*/
	class pipeline final
	{
	public:
		/** Binds vertices to pipeline.
		*	Pipeline will use them from rendering until different vertices will be binded or reset_state() called.
		*/
		void bind_vertices(std::vector<vector3> const* vertices);

		/** Binds indices to pipeline.
		*	Pipeline will use them from rendering until different indices will be binded or reset_state() called.
		*/
		void bind_indices(std::vector<unsigned int> const* indices);

		/** Binds color attribute to pipeline.
		*	Pipeline will use this attribute until reset_state() called.
		*/
		void bind_color_attribute(mesh_attribute_info<color> const* attr_info, color* bind_point);

		/** Binds vector2 attribute to pipeline.
		*	Pipeline will use this attribute until reset_state() called.
		*/
		void bind_vector2_attribute(mesh_attribute_info<vector2> const* attr_info, vector2* bind_point);

		/** Binds vector3 attribute to pipeline.
		*	Pipeline will use this attribute until reset_state() called.
		*/
		void bind_vector3_attribute(mesh_attribute_info<vector3> const* attr_info, vector3* bind_point);

		/** Binds shader to pipeline.
		*	Pipeline will use this shader until different one will be binded or until reset_state() called.
		*/
		void bind_shader(shader* s);

		/** Reset saved pipeline state.
		*	It discards binded shader, vertices, indices and all attributes.
		*/
		void reset_state();

		/** Runs with binded values using provided painter.
		*/
		void draw(bitmap_painter& painter);

	private:
		std::vector<vector3> const* m_vertices;
		std::vector<unsigned int> const* m_indices;

		std::vector<binded_mesh_attribute_info<color>> m_color_attributes;
		std::vector<binded_mesh_attribute_info<vector2>> m_vector2_attributes;
		std::vector<binded_mesh_attribute_info<vector3>> m_vector3_attributes;

		shader* m_shader;
	};
}

#endif // LANTERN_PIPELINE_H
