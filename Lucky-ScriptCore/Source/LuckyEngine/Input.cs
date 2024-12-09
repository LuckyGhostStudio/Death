namespace LuckyEngine
{
    public class Input
    {
        public static bool GetKeyDown(KeyCode key)
        {
            return InternalCalls.Input_GetKeyDown(key);
        }

        // TODO Other
    }
}
