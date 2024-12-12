using System;
using System.Runtime.CompilerServices;
using LuckyEngine;

namespace Game
{
    public class Player : MonoBehaviour
    {
        private Transform transform;

        public float speed = 1.0f;
        public float damage = 1.2f;

        public float time;

        void Awake()
        {
            Console.WriteLine("Player.Awake - {0}", gameObject.ID);
            Console.WriteLine("{0}", gameObject.HasComponent<Transform>());

            transform = gameObject.GetComponent<Transform>();

            speed = 2.22f;
        }

        void Update(float dt)
        {
            time += dt;

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

            transform.position += velocity * dt;
        }
    }
}
