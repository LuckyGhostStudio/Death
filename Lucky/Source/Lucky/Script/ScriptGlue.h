#pragma once

namespace Lucky
{
    /// <summary>
    /// �ű�ע���ע�� C++ ����ʵ�� ���� InternalCalls.cs ������ C# �������� 
    /// </summary>
    class ScriptGlue
    {
    public:
        /// <summary>
        /// ע���������
        /// </summary>
        static void RegisterComponents();

        /// <summary>
        /// ע���ڲ����ú���
        /// </summary>
        static void RegisterInternalCalls();
    };
}