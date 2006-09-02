#ifndef ION_SCENE_NODE_HH_INCLUDED
#define ION_SCENE_NODE_HH_INCLUDED

#include "../base/dll.hh"
#include "../base/refcounted.hh"
#include "../base/streamable.hh"
#include "../math/spherevolume.hh"
#include "../math/transform.hh"

namespace ion {
namespace scene {

	//! Scenegraph node class.
	/**
	* This is the basic building block of a scenegraph. In ion, a scenegraph handles node ownership and
	* transformation hierarchies. Reference counting is used heavily; adding a child to a node increases the
	* new child's reference count, nodes being deleted are automatically removed from their parents etc.
	*/
	class ION_API Node:public base::RefCounted
	{
	public:
		class ION_API TransformAlterationFunctor
		{
		public:
			virtual ~TransformAlterationFunctor();
			virtual void transformAltered(const bool rotationAltered,const bool positionAltered)=0;
		};

		~Node();

		const math::Transform& transform() const;
		virtual void invalidateGlobalTransform();

		const math::Transform& localTransform() const;
		virtual void localTransform(const math::Transform& localtransform);
		virtual void rotation(const math::Quaternion& newrotation);
		virtual void rotationFromRotAxisAndAngle(const math::Vector3f& axis,const float angle);
		virtual void rotationFromEulerAngles(const float x,const float y,const float z);
		virtual void position(const float x,const float y,const float z);
		virtual void position(const math::Vector3f& newposition);
		virtual void extractLocalTransformFrom(const math::Matrix4f& m);

		void inheritTransform(const bool inherit);
		bool inheritTransform() const;

		virtual const math::SphereVolume& boundingSphere() const;

		virtual const char *type() const=0;
		virtual void respawn(base::Streamable& source)=0;
		virtual void serialize(base::Streamable& dest)=0;

		virtual bool isEnabled() const;
		virtual void enable(const bool state);

		Node *parent();
		const Node *parent() const;

		unsigned long numChildren() const;
		Node* child(const unsigned long index);
		const Node* child(const unsigned long index) const;

		virtual void addChild(Node& rChild);
		void removeChild(Node& rChild);
		virtual void removeChild(Node& rChild,const bool useParentTransform);

		virtual void attachChild(Node& rChild);
		void detachChild(Node& rChild);
		virtual void detachChild(Node& rChild,const bool useParentTransform);

		bool hasChild(Node& rChild);

		void transformAlterationFunctor(TransformAlterationFunctor *pTransformAlterationFunctor);
		TransformAlterationFunctor* transformAlterationFunctor();

		struct InternalNodeData;
		InternalNodeData& internalNodeData();
	protected:
		Node(const base::String& identifier);

		mutable math::Transform m_Transform;
		math::Transform m_LocalTransform;
		mutable bool m_TransformValid;
		bool m_Enabled,m_InheritTransform;
		TransformAlterationFunctor *m_pTransformAlterationFunctor;

	private:
		InternalNodeData* m_pInternalNodeData;
	};

}
}

#endif
