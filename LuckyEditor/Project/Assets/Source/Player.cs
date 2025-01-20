using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using LuckyEngine;

namespace Game
{
    public enum AttributeType
    {
        Metal,  // (金)：与金属相关的属性，通常与坚硬、锋利、切割等特性相关联.
        Wood,   // (木)：与植物和生长相关的属性，通常与生命力、恢复和自然之力相关.
        Water,  // (水)：与水相关的属性，通常与流动、适应、治愈和冰冻等特性相关.
        Fire,   // (火)：与火焰相关的属性，通常与炽热、破坏、热情和能量等特性相关.
        Earth,  // (土)：与土地和岩石相关的属性，通常与坚固、稳定、防御和大地之力相关.

        Ice,    // (冰)：与冰相关的属性，通常与寒冷、冻结、冷静和控制等特性相关.
        Fog,    // (雾)：与雾相关的属性，通常与朦胧、隐蔽、迷惑和环境适应等特性相关

        // 风雷等
    }

    public enum AttributeState
    {
        Real,   // 实 = 虚 + 传承 = 伪 + 属性丹药
        Unreal, // 虚 = 心法 = 0 + 属性丹药
        Fake,   // 伪
    }

    public class ElementalAttribute
    {
        AttributeType m_Type;
        AttributeState m_State;
    }

    public class HeartMethod
    {
        private List<ElementalAttribute> m_Attributes;  // 心法的属性
    }

    public enum SkillType
    {
        // 防御 攻击 治疗等
    }

    public class Skill
    {
        private List<ElementalAttribute> m_Attributes;  // 技能的属性

        private SkillType m_Type;

        // 物理攻击 神识技能
    }

    public class Player : MonoBehaviour
    {
        private float m_TrueQiStage;
        private float m_BodyStage;
        private float m_SpiritStage;

        private List<ElementalAttribute> m_Attributes;

        private HeartMethod m_HeartMethod;
        private List<Skill> m_Skills;

        private Transform transform;

        public float speed = 1.0f;
        public float damage = 1.2f;

        public float time;

        void Awake()
        {
            Console.WriteLine("Player.Awake - {0}", gameObject.ID);
            Console.WriteLine("{0}", gameObject.HasComponent<Transform>());

            transform = gameObject.GetComponent<Transform>();

            //speed = 2.22f;
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
