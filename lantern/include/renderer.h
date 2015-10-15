#ifndef LANTERN_RENDERER_H
#define LANTERN_RENDERER_H

#include "shader_bind_point_info.h"
#include "geometry_stage.h"
#include "rasterizing_stage.h"
#include "merging_stage.h"
#include <stdexcept>

namespace lantern
{
	/** @defgroup Rendering */

	/** Renderer is the root object for rendering in a texture.
	* It manages all the stages and passes data between them.
	* There are three stages for now, in order of invoking:
	* - \ref geometry_stage is reponsible for transforming geometry.
	* - \ref rasterizing_stage is responsible for calculating polygon's coverage.
	* - \ref merging_stage is responsible for merging results into a texture
	* @ingroup Rendering
	*/
	class renderer final
	{
		friend class geometry_stage;
		friend class rasterizing_stage;
		friend class merging_stage;

	public:
		/** Constructs renderer with default settings for each stage */
		renderer();

		/** Gets geometry stage
		* @returns Geometry stage instance
		*/
		geometry_stage& get_geometry_stage();

		/** Gets rasterizing stage
		* @returns Rasterizing stage instance
		*/
		rasterizing_stage& get_rasterizing_stage();

		/** Gets merging stage
		* @returns Merging stage instance
		*/
		merging_stage& get_merging_stage();

		/** Renders a mesh in a texture using specified shader
		* @param mesh Mesh to render
		* @param shader Shader to use for rendering
		* @param target_texture Texture to render image into
		*/
		template<typename TShader>
		void render_mesh(mesh const& mesh, TShader& shader, texture& target_texture);

	private:
		/** Passes geometry stage result to the rasterizer stage
		* @param vertex0 First triangle vertex
		* @param vertex1 Second triangle vertex
		* @param vertex2 Third triangle vertex
		* @param index0 First vertex attribute index
		* @param index1 Second vertex attribute index
		* @param index2 Third vertex attribute index
		* @param shader Shader to use
		* @param target_texture Texture polygon will drawn into
		*/
		template<typename TShader>
		void process_geometry_stage_result(
			vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
			unsigned int const index0, unsigned int const index1, unsigned int const index2,
			TShader& shader,
			texture& target_texture);

		/** Passes rasterizing stage result to the merging stage
		* @param pixel_coordinates Coordinates of a pixel that should be filled
		* @param sample_point Sample point coordinates
		* @param shader Shader to use
		* @param target_texture Texture polygon will drawn into
		*/
		template<typename TShader>
		void process_rasterizing_stage_result(
			vector2ui const& pixel_coordinates, vector3f sample_point, TShader& shader, texture& target_texture);

		/** Binds mesh attributes to shader bind points
		* @param required_bind_points Shader bind points
		* @param available_attributes Mesh attributes
		* @param binded_attributes_storage Storage for bindings
		*/
		template<typename TAttr>
		void bind_attributes(
			std::vector<shader_bind_point_info<TAttr>> const& required_bind_points,
			std::vector<mesh_attribute_info<TAttr>> const& available_attributes,
			std::vector<binded_mesh_attribute_info<TAttr>>& binded_attributes_storage);

		/* Geometry stage instance */
		geometry_stage m_geometry_stage;

		/* Rasterizing stage instance */
		rasterizing_stage m_rasterizing_stage;

		/* Merging stage instance */
		merging_stage m_merging_stage;

		/** Binded attributes */
		binded_mesh_attributes m_binded_mesh_attributes;
	};

	template<typename TShader>
	inline void renderer::render_mesh(mesh const& mesh, TShader& shader, texture& target_texture)
	{
		// Prepare bind points for all available types
		//
		bind_attributes(shader.get_color_bind_points(), mesh.get_color_attributes(), m_binded_mesh_attributes.color_attributes);
		bind_attributes(shader.get_float_bind_points(), mesh.get_float_attributes(), m_binded_mesh_attributes.float_attributes);
		bind_attributes(shader.get_vector2f_bind_points(), mesh.get_vector2f_attributes(), m_binded_mesh_attributes.vector2f_attributes);
		bind_attributes(shader.get_vector3f_bind_points(), mesh.get_vector3f_attributes(), m_binded_mesh_attributes.vector3f_attributes);

		// Pass data to the first stage
		//
		bool const do_homogeneous_division{m_rasterizing_stage.get_rasterization_algorithm() == rasterization_algorithm_option::homogeneous};
		m_geometry_stage.invoke(mesh, shader, do_homogeneous_division, target_texture, *this);
	}

	template<typename TShader>
	inline void renderer::process_geometry_stage_result(
		vector4f const& vertex0, vector4f const& vertex1, vector4f const& vertex2,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		TShader& shader,
		texture& target_texture)
	{
		m_rasterizing_stage.invoke(
			vertex0, vertex1, vertex2,
			index0, index1, index2,
			shader,
			m_binded_mesh_attributes,
			target_texture,
			*this);
	}

	template<typename TShader>
	inline void renderer::process_rasterizing_stage_result(
		vector2ui const& pixel_coordinates, vector3f sample_point, TShader& shader, texture& target_texture)
	{
		m_merging_stage.invoke(pixel_coordinates, sample_point, shader, target_texture, *this);
	}

	template<typename TAttr>
	void renderer::bind_attributes(
		std::vector<shader_bind_point_info<TAttr>> const& required_bind_points,
		std::vector<mesh_attribute_info<TAttr>> const& available_attributes,
		std::vector<binded_mesh_attribute_info<TAttr>>& binded_attributes_storage)
	{
		// Clear the storage
		binded_attributes_storage.clear();

		size_t const bind_points_size{required_bind_points.size()};
		size_t const available_attributes_size{available_attributes.size()};

		for (size_t i{0}; i < bind_points_size; ++i)
		{
			shader_bind_point_info<TAttr> const& bind_point_info = required_bind_points.at(i);

			bool binded{false};

			// Find mesh attribute we can bind to the point
			//
			for (size_t j{0}; j < available_attributes_size; ++j)
			{
				mesh_attribute_info<TAttr> const& attr_info = available_attributes.at(j);
				if (attr_info.get_id() == bind_point_info.attribute_id)
				{
					binded_attributes_storage.push_back(
						binded_mesh_attribute_info<TAttr>{attr_info, bind_point_info.bind_point});

					binded = true;
					break;
				}
			}

			if (!binded)
			{
				throw std::runtime_error("Mesh doesn't contain attribute required by shader");
			}
		}
	};
}

#endif // LANTERN_RENDERER_H