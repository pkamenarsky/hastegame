#ifndef ION_MATH_TRANSFORM_HH_INCLUDED
#define ION_MATH_TRANSFORM_HH_INCLUDED

#include "matrix.hh"
#include "quaternion.hh"
#include "vectors.hh"

namespace ion {
namespace math {

	//! Transformation class
	/**
	* Instead of using matrices, this class is often preferred. It consists of a quaternion (representing
	* the rotation) and a vector (representing translation). This allows easy interpolation, gimbal-lock-free
	* rotation and absence of other euler rotation problems, and more. In case its needed, a matrix
	* representationcan be retrieved using the matrix() getter. extractFrom() does the opposite (extracts
	* rotation & position out of a given matrix). Convenience lookat methods are also provided.
	*/
	class ION_API Transform
	{
	public:
		//! Default constructor with identity quaternion and position (0,0,0) set
		Transform();
		//! Constructor for explicit setting of rotation & position
		Transform(const Vector3f& position,const Quaternion& rotation);
		//! Copy constructor. See clone() for more.
		/**
		* @param src The transform to be copied.
		*/
		Transform(const Transform &src);

		//! Clones the given transform.
		/*
		* Position and rotation of the "src" transform are transferred to this transform,
		* thus making an exact duplicate.
		* @param src The transform to be copied.
		*/
		void clone(const Transform &src);
		//! Clones the given transform. See clone() for more.
		/**
		* @param src The transform to be copied.
		*/
		Transform& operator =(const Transform& src);

		//! Sets the rotation to the specified value.
		/**
		* @param newrotation New rotation value for this transform.
		*/
		void rotation(const Quaternion& newrotation);
		//! Retrieves a reference to this transform's rotation.
		/**
		* Since the reference is non-const, the rotation can be manipulated using the return value.
		* @return The rotation quaternion.
		*/
		Quaternion& rotation();
		//! Retrieves a const reference to this transform's rotation.
		/**
		* Since the reference is const, the rotation cannot be manipulated using the return value. It is read-only.
		* @return The rotation quaternion.
		*/
		const Quaternion& rotation() const;

		//! Sets the position to the specified value.
		/**
		* @param x New x position coordinate.
		* @param y New y position coordinate.
		* @param z New z position coordinate.
		*/
		void position(const float x,const float y,const float z);
		//! Sets the position to the specified value.
		/**
		* @param newposition New position coordinate for this transform.
		*/
		void position(const Vector3f& newposition);
		//! Retrieves a reference to this transform's position.
		/**
		* Since the reference is non-const, the position can be manipulated using the return value.
		* @return The position vector.
		*/
		Vector3f& position();
		//! Retrieves a reference to this transform's position.
		/**
		* Since the reference is const, the position cannot be manipulated using the return value. It is read-only.
		* @return The position vector.
		*/
		const Vector3f& position() const;

		//! Calculates and returns a const reference to a transformation matrix.
		/**
		* Use this if you need a matrix representation of the transform.
		* Internally, a matrix is calculated; the return value is a const reference to this matrix.
		* Since it is const, access to the matrix is read-only. The matrix is only recalculated when
		* this method is called and the transform changed after the last time matrix() was called,
		* thereby preventing redundant recalculations.
		* @return Const reference to the transform matrix representation.
		*/
		const Matrix4f& matrix() const;
		//! Extracts position & rotation values from the given matrix.
		/**
		* @param m Matrix whose rotation & position transformations are to be extracted.
		*/
		void extractFrom(const Matrix4f& m);

		//! Lookat-Method useful for camera handling.
		/**
		* This method expects three parameters; origin (where the camera is located),
		* out (view direction), worldUp (tells the method how "up" is defined).
		* A transform is generated that matches the given parameters.
		* @param origin Location of the viewer / camera.
		* @param out View direction.
		* @param worldUp Up direction.
		*/
		void lookatDirection(const Vector3f& origin,const Vector3f& out,const Vector3f& worldUp);
		//! Lookat-Method useful for camera handling.
		/**
		* This method expects three parameters; origin (where the camera is located),
		* to (the point the camera is looking at), worldUp (tells the method how "up" is defined).
		* A transform is generated that matches the given parameters.
		* This method is similar to lookatDirection(), but it expects a look target instead of a
		* view direction.
		* @param origin Location of the viewer / camera.
		* @param to Point the camera / viewer is looking at.
		* @param worldUp Up direction.
		*/
		void lookatLocation(const Vector3f& origin,const Vector3f& to,const Vector3f& worldUp);

	protected:
		Vector3f m_Position;
		Quaternion m_Rotation;
		mutable bool m_MatrixUpToDate;
		mutable Matrix4f m_Matrix;
	};

}
}

#endif
