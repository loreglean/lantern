#ifndef LANTERN_MERGING_STAGE_H
#define LANTERN_MERGING_STAGE_H

#include "vector2.h"
#include "vector3.h"
#include "texture.h"

namespace lantern
{
	/** This stage is responsible for invoking pixel shader and merging results into a texture */
	class merging_stage final
	{
	public:
		/** Constructs merging stage with default settings */
		merging_stage();

		/** Gets alpha blending mode
		* @returns Alpha blending mode
		*/
		bool get_alpha_blending_enabled() const;

		/** Sets alpha blending mode
		* @param enabled Alpha blending mode
		*/
		void set_alpha_blending_enabled(bool const enabled);

		/** Invokes stage
		* @param point Point coordinates to process
		* @param shader Shader to invoke
		* @param target_texture Texture to merge results into
		* @param delegate Delegate to pass results to for futher processing
		*/
		template<typename TShader, typename TDelegate>
		void invoke(vector2ui const& pixel_coordinates, vector3f const& sample_point, TShader& shader, texture& target_texture, TDelegate& delegate);

	private:
		/** True = use alpha channel during merging */
		bool m_alpha_blending_enabled;
	};

	template<typename TShader, typename TDelegate>
	inline void merging_stage::invoke(vector2ui const& pixel_coordinates, vector3f const& sample_point, TShader& shader, texture& target_texture, TDelegate& delegate)
	{
		color const color_from_shader = shader.process_pixel(pixel_coordinates);

		if (!m_alpha_blending_enabled)
		{
			target_texture.set_pixel_color(pixel_coordinates, color_from_shader);
		}
		else
		{
			color const current_texture_color = target_texture.get_pixel_color(pixel_coordinates);
			color const blended_color = color_from_shader * color_from_shader.a + current_texture_color * (1.0f - color_from_shader.a);
			target_texture.set_pixel_color(pixel_coordinates, blended_color);
		}
	}
}

#endif // LANTERN_MERGING_STAGE_H