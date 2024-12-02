#pragma once

namespace Lucky
{
    /// <summary>
    /// �ű����棺���� CSharp Mono 
    /// </summary>
    class ScriptEngine
    {
    public:
        /// <summary>
        /// ��ʼ��
        /// </summary>
        static void Init();

        /// <summary>
        /// ����
        /// </summary>
        static void Shutdown();
    private:
        /// <summary>
        /// ��ʼ�� Mono ����ʱ
        /// </summary>
        static void InitMono();

        /// <summary>
        /// ���� Mono ����ʱ
        /// </summary>
        static void ShutdownMono();
    };
}