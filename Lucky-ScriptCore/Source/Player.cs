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
        float speed = 1.0f;

        void Awake()
        {
            Console.WriteLine("Player.Awake - {0}", ID);
        }

        void Update(float dt)
        {
            Console.WriteLine("Player.Update: {0}", dt);

            Vector3 velocity = Vector3.Zero;

            if (Input.GetKeyDown(KeyCode.A))
            {
                velocity.x = -1;
            }
            else if (Input.GetKeyDown(KeyCode.D))
            {
                velocity.x = 1;
            }

            if (Input.GetKeyDown(KeyCode.W))
            {
                velocity.y = 1;
            }
            else if (Input.GetKeyDown(KeyCode.S))
            {
                velocity.y = -1;
            }

            velocity *= speed;

            Vector3 pos = Position;
            pos += velocity * dt;
            Position = pos;
        }
    }
}
