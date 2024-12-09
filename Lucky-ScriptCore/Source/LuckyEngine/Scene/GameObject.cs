using System;
namespace LuckyEngine
{
    public class GameObject
    {
        internal GameObject(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;   // Object::UUID

        public GameObject gameObject => this;

        // TODO Scene

        // TODO Transform

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.GameObject_HasComponent(ID, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
            {
                return null;
            }

            T component = new T() { gameObject = this };    // TODO 可能添加缓存 下次不需要 new
            return component;
        }
    }
}
