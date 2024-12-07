using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LuckyEngine;

namespace Game
{
    public class Player : MonoBehaviour
    {
        void Awake()
        {
            Console.WriteLine("Player.Awake");
        }

        void Update(float dt)
        {
            Console.WriteLine("Player.Update: {0}", dt);
        }
    }
}
