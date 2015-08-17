#ifndef LANTERN_UI_ELEMENT_BASE_H
#define LANTERN_UI_ELEMENT_BASE_H

#include "pipeline.h"

namespace lantern
{
	/** Base class for all the UI elements.
	* @ingroup UI
	*/
	class ui_element_base
	{
	public:
		/** Virtual destructor */
		virtual ~ui_element_base();

		/** Sets NDC position (left-bottom point)
		* @param position New position
		*/
		void set_position(vector2f position);

		/** Draws the element and all its children
		* @param pipeline Pipeline to use
		* @param target_texture Texture to draw to
		*/
		virtual void draw(pipeline& pipeline, texture& target_texture);

		/** Gets children storage
		* @returns Children storage
		*/
		std::vector<ui_element_base*> get_children_storage();

	protected:
		/** Invokes whenever position is changed */
		virtual void on_position_changed();

		/** NDC position */
		vector2f m_position;

		/** Subelements */
		std::vector<ui_element_base*> m_children;
	};
}

#endif // LANTERN_UI_ELEMENT_BASE_H