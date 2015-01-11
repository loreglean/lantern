#ifndef LANTERN_VECTOR3_H
#define LANTERN_VECTOR3_H

#include <cmath>

namespace lantern
{
	/** Class representing 3-dimensional vector.
	* Can be interpreted either as a point or a geometrical vector, providing operations for both interpretations.
	* It also can be used to just store three related float values
	*/
	class vector3 final
	{
	public:
		/** X-coordinate */
		float x;

		/** Y-coordinate */
		float y;

		/** Z-coordinate */
		float z;

		/** Constructs vector with zero values */
		vector3();

		/** Constructs vector with specified values
		* @param x X-coordinate
		* @param y Y-coordinate
		* @param z Z-coordinate
		*/
		vector3(float const x, float const y, float const z);

		vector3 operator+(vector3 const& v) const;
		vector3& operator+=(vector3 const& v);
		vector3 operator-(vector3 const& v) const;
		vector3& operator-=(vector3 const& v);
		vector3 operator-() const;
		vector3 operator*(float const s) const;
		vector3& operator*=(float const s);
		vector3 operator/(float const s) const;
		vector3& operator/=(float const s);

		/** Calculates vector's length
		* @returns Length
		*/
		float length() const;

		/** Calculates vector's squared length, thus avoiding square root operation
		* @returns Squared length
		*/
		float length_sqr() const;

		/** Normalizes vector (changing length to 1) */
		void normalize();

		/** Gets normalized vector, leaving this vector untouched
		* @returns Normalized version of this vector
		*/
		vector3 normalized() const;

		/** Calculates dot product with another vector
		* @returns Dot product
		*/
		float dot(vector3 const& v) const;

		/** Calculates cross product with another vector
		* @returns Cross product
		*/
		vector3 cross(vector3 const& v) const;

		/** Calculates distance to another point
		* @returns Distance
		*/
		float distance_to(vector3 const& p) const;

		/** Calculates angle with another vector
		* @returns Angle in radians
		*/
		float angle_with(vector3 const& v) const;

		/** Calculates projection of this vector on another vector
		* @returns Projection vector
		*/
		vector3 projection_on(vector3 const& v) const;

		/** Calculates perpendicular from this vector to another vector
		* @returns Perpendicular
		*/
		vector3 perpendicular_on(vector3 const& v) const;

		static vector3 const ZERO;
		static vector3 const X_UNIT;
		static vector3 const Y_UNIT;
		static vector3 const Z_UNIT;
	};

	vector3 operator*(float const s, vector3 const& v);
}

#endif // LANTERN_VECTOR3_H
