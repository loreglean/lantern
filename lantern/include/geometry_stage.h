#ifndef LANTERN_GEOMETRY_STAGE_H
#define LANTERN_GEOMETRY_STAGE_H

#include "mesh.h"
#include "texture.h"
#include "matrix4x4.h"

namespace lantern
{
	/** This rendering stage is responsible for transforming geometry and invoking a vertex shader
	* @ingroup Rendering
	*/
	class geometry_stage final
	{
	public:
		/** Constructs geometry stage with default settings */
		geometry_stage();

		/** Invokes stage
		* @param mesh Mesh to process
		* @param shader Shader to use for vertex processing
		* @param target_texture Texture mesh will be rendered to
		* @param delegate Object to pass results to for futher processing
		*/
		template<typename TShader, typename TDelegate>
		void invoke(
			mesh const& mesh,
			TShader& shader,
			bool const do_homogeneous_division,
			texture& target_texture,
			TDelegate& delegate);

	private:
		/** Storage for transformed vertices */
		std::vector<vector4f> m_transformed_vertices_storage;

		/* Storage for clip flags */
		std::vector<bool> m_transformed_vertices_clip_flags_storage;
	};

	template<typename TShader, typename TDelegate>
	void geometry_stage::invoke(
		mesh const& mesh,
		TShader& shader,
		bool const do_homogeneous_division,
		texture& target_texture,
		TDelegate& delegate)
	{
		// Change transformed vertices storages capacity if needed and clear it
		//

		std::vector<vector3f> const& vertices = mesh.get_vertices();
		size_t vertices_count{vertices.size()};

		if (m_transformed_vertices_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_storage.reserve(vertices_count);
		}
		m_transformed_vertices_storage.clear();

		if (m_transformed_vertices_clip_flags_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_clip_flags_storage.reserve(vertices_count);
		}
		m_transformed_vertices_clip_flags_storage.clear();

		// Process vertices and clip
		//
		for (size_t i{0}; i < vertices_count; ++i)
		{
			vector3f const& v{vertices.at(i)};
			vector4f v_transformed{shader.process_vertex(vector4f{v.x, v.y, v.z, 1.0f})};

			bool clipped{false};
			if ((v_transformed.x > v_transformed.w) || (v_transformed.x < -v_transformed.w))
			{
				clipped = true;
			}
			else if ((v_transformed.y > v_transformed.w) || (v_transformed.y < -v_transformed.w))
			{
				clipped = true;
			}
			else if ((v_transformed.z > v_transformed.w) || (v_transformed.z < -v_transformed.w))
			{
				clipped = true;
			}

			m_transformed_vertices_storage.push_back(v_transformed);
			m_transformed_vertices_clip_flags_storage.push_back(clipped);
		}

		// Transform vertices to screen coordinates
		//

		float const width{static_cast<float>(target_texture.get_width())};
		float const height{static_cast<float>(target_texture.get_height())};

		if (do_homogeneous_division)
		{
			matrix4x4f const ndc_to_screen{
				(width) / 2.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -(height) / 2.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				(width) / 2.0f, (height) / 2.0f, 0.0f, 1.0f};

			for (size_t i{0}; i < vertices_count; ++i)
			{
				vector4f& v = m_transformed_vertices_storage.at(i);
				v = v * ndc_to_screen;
			}
		}
		else
		{
			for (size_t i{0}; i < vertices_count; ++i)
			{
				if (m_transformed_vertices_clip_flags_storage.at(i) == true)
				{
					continue;
				}

				vector4f& v = m_transformed_vertices_storage.at(i);

				float const w_inversed{1.0f / v.w};

				v.x *= w_inversed;
				v.y *= w_inversed;
				v.z *= w_inversed;

				v.w = w_inversed;

				// NDC to screen
				//
				v.x = v.x * width / 2.0f + width / 2.0f;
				v.y = -v.y * height / 2.0f + height / 2.0f;
			}
		}
		
		// Process results
		//
		std::vector<unsigned int> const& indices = mesh.get_indices();

		size_t indices_count{indices.size()};
		for (size_t i{0}; i < indices_count; i += 3)
		{
			unsigned int const index0{indices.at(i + 0)};
			unsigned int const index1{indices.at(i + 1)};
			unsigned int const index2{indices.at(i + 2)};

			vector4f const& v0 = m_transformed_vertices_storage.at(index0);
			vector4f const& v1 = m_transformed_vertices_storage.at(index1);
			vector4f const& v2 = m_transformed_vertices_storage.at(index2);

			bool const v0_clipped{m_transformed_vertices_clip_flags_storage.at(index0)};
			bool const v1_clipped{m_transformed_vertices_clip_flags_storage.at(index1)};
			bool const v2_clipped{m_transformed_vertices_clip_flags_storage.at(index2)};

			// Clip triangle if at least one vertex is outside of a screen as a workaround
			// TODO: clipping
			//
			if (v0_clipped || v1_clipped || v2_clipped)
			{
				continue;
			}

			delegate.process_geometry_stage_result(
				v0, v1, v2,
				index0, index1, index2,
				shader,
				target_texture);
		}
	}
}

#endif // LANTERN_GEOMETRY_STAGE_H