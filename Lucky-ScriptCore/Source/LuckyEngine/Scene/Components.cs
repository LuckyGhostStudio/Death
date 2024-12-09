namespace LuckyEngine
{
    public abstract class Component
    {
        // 该组件对应的实体对象 GameObject, 在 MonoBehaviour 中初始化
        public GameObject gameObject { get; internal set; }
    }

    public class Transform : Component
    {
        public Vector3 position
        {
            get
            {
                InternalCalls.Transform_GetPosition(gameObject.ID, out Vector3 ret);
                return ret;
            }
            set
            {
                InternalCalls.Transform_SetPosition(gameObject.ID, ref value);
            }
        }

        // TODO rotation
        // TODO scale
        
        // TODO localPosition
        // TODO localRotation
        // TODO localScale
    }
}
