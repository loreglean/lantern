#ifndef LANTERN_OBJ_IMPORT_H
#define LANTERN_OBJ_IMPORT_H

#include <string>
#include "mesh.h"

namespace lantern
{
	/** Wavefront .obj file reader.
	* Derived classes can override on_* virtual functions to process definitions
	*/
	class obj_reader
	{
	public:
		/** Parses .obj file
		* @param path Path to obj file
		*/
		void read(std::string const& path);

		typedef void (obj_reader::*parse_face_function)(std::istringstream&);

	protected:
		/** Gets called when parsing starts */
		virtual void on_reading_started();

		/** Gets called when parsing ends */
		virtual void on_reading_ended();

		/** Gets called when vertex definition found
		* @param x Vertex x-coordinate
		* @param y Vertex y-coordinate
		* @param z Vertex z-coordinate
		*/
		virtual void on_vertex_def(float const x, float const y, float const z);

		/** Gets called when texture coordinates definition found
		* @param x U-coordinate
		* @param y V-coordinate
		*/
		virtual void on_texcoord_def(float const x, float const y);

		/** Gets called when normal definition found
		* @param x Normal x-coordinate
		* @param y Normal y-coordinate
		* @param z Normal z-coordinate
		*/
		virtual void on_normal_def(float const x, float const y, float const z);

		/** Gets called when face definition parsing starts */
		virtual void on_face_def_started();

		/** Gets called when face definition parsing ends */
		virtual void on_face_def_ended();

		/** Gets called when face indices are ready
		* @param vertex_index0 Index of first vertex
		* @param vertex_index1 Index of second vertex
		* @param vertex_index2 Index of third vertex
		*/
		virtual void on_face_pos_def(unsigned int vertex_index0, unsigned int vertex_index1, unsigned int vertex_index2);

		/** Gets called when face texcoords are ready
		* @param texcoord_index0 Index of first texcoord
		* @param texcoord_index1 Index of second texcoord
		* @param texcoord_index2 Index of third texcoord
		*/
		virtual void on_face_texcoord_def(unsigned int texcoord_index0, unsigned int texcoord_index1, unsigned int texcoord_index2);

		/** Gets called when face normals are ready
		* @param normal_index0 Index of first normal
		* @param normal_index1 Index of second normal
		* @param normal_index2 Index of third normal
		*/
		virtual void on_face_normal_def(unsigned int normal_index0, unsigned int normal_index1, unsigned int normal_index2);

	private:
		/** Gets function suitable to parse specified definition
		* @param face_def Face definition string
		* @returns Function to parse the definition
		*/
		parse_face_function get_parse_function(std::string face_def);

		/** Parses vertex only face defintiion
		* @param stream Face definition string stream
		*/
		void parse_face_vertex(std::istringstream& stream);

		/** Parses vertex and normal face defintiion
		* @param stream Face definition string stream
		*/
		void parse_face_vertex_normal(std::istringstream& stream);

		/** Parses vertex and texcoord face defintiion
		* @param stream Face definition string stream
		*/
		void parse_face_vertex_texcoord(std::istringstream& stream);

		/** Parses vertex, texcoord and normal face defintiion
		* @param stream Face definition string stream
		*/
		void parse_face_vertex_texcoord_normal(std::istringstream& stream);
	};

	/** Simple importer of mesh object from .obj file
	*/
	class obj_mesh_importer final : public obj_reader
	{
	public:
		/** Constructs mesh importer with specified params
		* @param read_texcoords Indicates if mesh will contain texcoords from .obj file
		* @param read_normals Indicates if mesh will contain normals from .obj file
		*/
		obj_mesh_importer(bool const read_texcoords, bool const read_normals);

		/** Gets result mesh
		* @returns Mesh from .obj file
		*/
		mesh get_mesh() const;

		virtual void on_reading_started() override;

		virtual void on_vertex_def(float const x, float const y, float const z) override;
		virtual void on_texcoord_def(float const x, float const y) override;
		virtual void on_normal_def(float const x, float const y, float const z) override;

		virtual void on_face_pos_def(unsigned int vertex_index0, unsigned int vertex_index1, unsigned int vertex_index2) override;
		virtual void on_face_texcoord_def(unsigned int texcoord_index0, unsigned int texcoord_index1, unsigned int texcoord_index2) override;
		virtual void on_face_normal_def(unsigned int normal_index0, unsigned int normal_index1, unsigned int normal_index2) override;

	private:
		/** Is object reading texcoords */
		bool const m_read_texcoords;

		/** Is object reading normals */
		bool const m_read_normals;

		/** Mesh vertices */
		std::vector<vector3f> m_vertices;

		/** Mesh indices */
		std::vector<unsigned int> m_indices;

		/** Mesh texcoords */
		std::vector<vector2f> m_texcoords;

		/** Mesh texcoords indices */
		std::vector<unsigned int> m_texcoords_indices;

		/** Mesh normals */
		std::vector<vector3f> m_normals;

		/** Mesh normals indices */
		std::vector<unsigned int> m_normals_indices;
	};

	/** Reads mesh from .obj file. Helper function to avoid creating temporary parser objects
	* @param path .obj file path
	* @param read_texcoords Indicates if mesh will contain texcoords from .obj file
	* @param read_normals  Indicates if mesh will contain normals from .obj file
	*/
	mesh load_mesh_from_obj(std::string const& path, bool const read_texcoords, bool const read_normals);
}

#endif // LANTERN_OBJ_IMPORT_H
