using System;

namespace LuckyEngine
{
    public class Main
    {
        public float FloatVar { get; set; }

        public Main()
        {
            Console.WriteLine("Main Constructor.");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello LuckyEngine from C#.");
        }

        public void PrintInt(int value)
        {
            Console.WriteLine($"C#: {value}");
        }

        public void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C#: {value1} and {value2}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C#: {message}");
        }
    }
}
