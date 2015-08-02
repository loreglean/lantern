#ifndef LANTERN_SHADER_H
#define LANTERN_SHADER_H

namespace lantern
{
	/** @defgroup Shaders
	* Shader is a class representing shading algorithm.
	* It should not be treated as something "running on GPU" despite of common notation.
	* Its purpose is to move shading algorithm into separate object.
	* They do not derive from any base class to avoid virtual functions calls and allow inlining.
	* So they all should comply the same interface, \ref color_shader is an example
	*/

	/** Represents shader bind point information, holds required attribute ID and bind point itself.
	* Bind point is just an address of variable for rasterizer to put interpolated value into
	* @ingroup Shaders
	*/
	template<typename TAttr>
	class shader_bind_point_info
	{
	public:
		/** Attribute id */
		unsigned int attribute_id;

		/** Address to put value into */
		TAttr* bind_point;
	};
}

#endif // LANTERN_SHADER_H