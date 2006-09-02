#include <algorithm>
#include <vector>
#include "node.hh"

namespace ion {
namespace scene {

	struct Node::InternalNodeData
	{
		// The pair stores as first value the Node pointer; second value is a bool indicating whether reference counting shall be performed or not
		std::vector < std::pair < Node*, bool > > m_Children;
		Node *m_pParent;
		bool m_PositionAltered,m_RotationAltered;

		InternalNodeData():m_pParent(0) {}

		std::vector < std::pair < Node*, bool > >::iterator find(Node &rNode)
		{
			//return std::find(m_Children.begin(),m_Children.end(),&rNode);
			std::vector < std::pair < Node*, bool > >::iterator it=m_Children.begin();
			for (;it!=m_Children.end();++it) {
				if (it->first == &rNode)
					return it;
			}

			return m_Children.end();
		}

		void addNode(Node &rNode,const bool refcounting)
		{
			if (find(rNode)==m_Children.end()) {
				m_Children.push_back(std::pair < Node*, bool >(&rNode,refcounting));
			}
		}

		void removeNode(Node &rNode)
		{
			std::vector < std::pair < Node*, bool > >::iterator it=find(rNode);
			if (it!=m_Children.end()) {
				m_Children.erase(it);
			}
		}
	};

	Node::TransformAlterationFunctor::~TransformAlterationFunctor()
	{
	}

	Node::Node(const base::String& identifier):RefCounted(identifier),m_TransformValid(false),
	m_Enabled(true),m_InheritTransform(true),m_pTransformAlterationFunctor(0),m_pInternalNodeData(new InternalNodeData)
	{
	}

	void Node::transformAlterationFunctor(TransformAlterationFunctor *pTransformAlterationFunctor)
	{
		m_pTransformAlterationFunctor=pTransformAlterationFunctor;
	}

	Node::TransformAlterationFunctor* Node::transformAlterationFunctor()
	{
		return m_pTransformAlterationFunctor;
	}

	Node::~Node()
	{
		while (1) {
			std::vector < std::pair < Node*, bool > >::iterator it=m_pInternalNodeData->m_Children.begin();
			if (it==m_pInternalNodeData->m_Children.end()) break;

			Node *pNode=it->first;
			m_pInternalNodeData->m_Children.erase(it);
			pNode->internalNodeData().m_pParent=0;

			if (it->second)
				pNode->release();
		}

		if (m_pInternalNodeData->m_pParent!=0) {
			Node *pParent=m_pInternalNodeData->m_pParent;
			m_pInternalNodeData->m_pParent=0;
			pParent->internalNodeData().removeNode(*this);
		}
	
		delete m_pInternalNodeData;
	}

	const math::Transform& Node::transform() const
	{
		if (!m_TransformValid) {
			const Node *pParent=parent();

			if ((pParent!=0) && m_InheritTransform) {
				// TODO: this transform combination should have its own function
				math::Matrix4f m=m_LocalTransform.matrix()*pParent->transform().matrix();
				m_Transform.extractFrom(m);
				//if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(true,true);
			} else m_Transform=m_LocalTransform;

			m_TransformValid=true;
		}
		return m_Transform;
	}

	void Node::invalidateGlobalTransform()
	{
		if (!m_TransformValid) return;

		m_TransformValid=false;
		std::vector < std::pair < Node*, bool > >::iterator it=m_pInternalNodeData->m_Children.begin();
		for (;it!=m_pInternalNodeData->m_Children.end();++it)
			it->first->invalidateGlobalTransform();
	}

	const math::Transform&  Node::localTransform() const
	{
		return m_LocalTransform;
	}

	void Node::localTransform(const math::Transform& localtransform)
	{
		invalidateGlobalTransform();
		m_LocalTransform=localtransform;
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(true,true);
	}

	void Node::rotation(const math::Quaternion& newrotation)
	{
		invalidateGlobalTransform();
		m_LocalTransform.rotation(newrotation);
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(true,false);
	}

	void Node::rotationFromRotAxisAndAngle(const math::Vector3f& axis,const float angle)
	{
		invalidateGlobalTransform();
		m_LocalTransform.rotation(math::Quaternion::generateFromRotAxisAndAngle(axis,angle));
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(true,false);
	}

	void Node::rotationFromEulerAngles(const float x,const float y,const float z)
	{
		invalidateGlobalTransform();
		m_LocalTransform.rotation(math::Quaternion::generateFromEulerAngles(x,y,z));
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(true,false);
	}

	void Node::position(const float x,const float y,const float z)
	{
		invalidateGlobalTransform();
		m_LocalTransform.position(x,y,z);
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(false,true);
	}

	void Node::position(const math::Vector3f& newposition)
	{
		invalidateGlobalTransform();
		m_LocalTransform.position(newposition);
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(false,true);
	}

	void Node::extractLocalTransformFrom(const math::Matrix4f& m)
	{
		invalidateGlobalTransform();
		m_LocalTransform.extractFrom(m);
		if (m_pTransformAlterationFunctor) m_pTransformAlterationFunctor->transformAltered(true,true);
	}

	void Node::inheritTransform(const bool inherit)
	{
		if (m_InheritTransform==inherit) return;

		m_InheritTransform=inherit;
		if (parent()!=0) invalidateGlobalTransform();
	}

	bool Node::inheritTransform() const
	{
		return m_InheritTransform;
	}

	const math::SphereVolume& Node::boundingSphere() const
	{
		static math::SphereVolume s;
		return s;
	}

	bool Node::isEnabled() const
	{
		return m_Enabled;
	}

	void Node::enable(const bool state)
	{
		m_Enabled=state;
	}

	Node* Node::parent()
	{
		return m_pInternalNodeData->m_pParent;
	}

	const Node* Node::parent() const
	{
		return m_pInternalNodeData->m_pParent;
	}

	unsigned long Node::numChildren() const
	{
		return (unsigned long)(m_pInternalNodeData->m_Children.size());
	}

	Node* Node::child(const unsigned long index)
	{
		return (index<numChildren()) ? m_pInternalNodeData->m_Children[index].first : 0;
	}

	const Node* Node::child(const unsigned long index) const
	{
		return (index<numChildren()) ? m_pInternalNodeData->m_Children[index].first : 0;
	}

	void Node::addChild(Node& rChild)
	{
		if (hasChild(rChild)) return;

		m_pInternalNodeData->m_Children.push_back( std::pair < Node*, bool >(&rChild,true));

		if (rChild.internalNodeData().m_pParent!=0) {
			Node *pOldParent=rChild.internalNodeData().m_pParent;
			pOldParent->internalNodeData().removeNode(rChild);
		} else rChild.addRef();

		rChild.internalNodeData().m_pParent=this;
		rChild.invalidateGlobalTransform();
	}

	void Node::removeChild(Node& rChild)
	{
		removeChild(rChild,false);
	}

	void Node::removeChild(Node& rChild,const bool useParentTransform)
	{
		std::vector < std::pair < Node*, bool > >::iterator it=m_pInternalNodeData->find(rChild);
		if (it!=m_pInternalNodeData->m_Children.end()) {
			m_pInternalNodeData->m_Children.erase(it);

			// If useParentTransform is true and rChild.inheritTransform() is true, set rChild's local transform to its global transform
			// So that after detaching the child has the same transform in world space (useful for breaking off parts of a vehicle, for example)
			// If rChild.inheritTransform() is false this is pointless since then the parent transform is ignored anyway
			if (useParentTransform && rChild.inheritTransform()) {
				math::Transform t=rChild.transform();
				rChild.internalNodeData().m_pParent=0;
				rChild.localTransform(t);
			} else rChild.internalNodeData().m_pParent=0;

			rChild.invalidateGlobalTransform();
			rChild.release();
		}
	}

	void Node::attachChild(Node& rChild)
	{
		if (hasChild(rChild)) return;

		m_pInternalNodeData->m_Children.push_back(std::pair < Node*, bool >(&rChild,false));

		if (rChild.internalNodeData().m_pParent!=0) {
			Node *pOldParent=rChild.internalNodeData().m_pParent;
			pOldParent->internalNodeData().removeNode(rChild);
		}

		rChild.internalNodeData().m_pParent=this;
		rChild.invalidateGlobalTransform();
	}

	void Node::detachChild(Node& rChild)
	{
		detachChild(rChild,false);
	}

	void Node::detachChild(Node& rChild,const bool useParentTransform)
	{
		std::vector < std::pair < Node*, bool > >::iterator it=m_pInternalNodeData->find(rChild);
		if (it!=m_pInternalNodeData->m_Children.end()) {
			m_pInternalNodeData->m_Children.erase(it);

			// If useParentTransform is true and rChild.inheritTransform() is true, set rChild's local transform to its global transform
			// So that after detaching the child has the same transform in world space (useful for breaking off parts of a vehicle, for example)
			// If rChild.inheritTransform() is false this is pointless since then the parent transform is ignored anyway
			if (useParentTransform && rChild.inheritTransform()) {
				math::Transform t=rChild.transform();
				rChild.internalNodeData().m_pParent=0;
				rChild.localTransform(t);
			} else rChild.internalNodeData().m_pParent=0;

			rChild.invalidateGlobalTransform();
		}
	}

	bool Node::hasChild(Node& rChild)
	{
		return m_pInternalNodeData->find(rChild) != m_pInternalNodeData->m_Children.end();
	}

	Node::InternalNodeData& Node::internalNodeData()
	{
		return *m_pInternalNodeData;
	}

}
}

