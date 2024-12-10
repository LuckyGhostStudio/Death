using System;
using LuckyEngine;

namespace Game
{
    public class Player : MonoBehaviour
    {
        private Transform transform;

        float speed = 1.0f;

        void Awake()
        {
            Console.WriteLine("Player.Awake - {0}", gameObject.ID);
            Console.WriteLine("{0}", gameObject.HasComponent<Transform>());

            transform = gameObject.GetComponent<Transform>();
        }

        void Update(float dt)
        {
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
