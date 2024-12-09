namespace LuckyEngine
{
    public class MonoBehaviour : Component
    {
        protected MonoBehaviour()
        {
            gameObject = new GameObject(0);
        }

        /// <summary>
        /// 在 ScriptEngine 中调用
        /// </summary>
        /// <param name="id">Object::UUID</param>
        internal MonoBehaviour(ulong id)
        {
            gameObject = new GameObject(id);    // 创建 GameObject
        }

        //public readonly ulong ID;
    }
}
