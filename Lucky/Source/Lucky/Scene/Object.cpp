#include "lcpch.h"
#include "Object.h"

namespace Lucky
{
	Object::Object(entt::entity objectID, Scene* scene)
		: m_ObjectID(objectID), m_Scene(scene)
	{

	}
}