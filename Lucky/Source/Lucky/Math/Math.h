#pragma once

#include <glm/glm.hpp>

namespace Lucky::Math
{
    /// <summary>
    /// �ֽ� Transform ����
    /// </summary>
    /// <param name="transform">Transform ����</param>
    /// <param name="outPosition">���λ������</param>
    /// <param name="outRotation">�����ת����</param>
    /// <param name="outScale">�����������</param>
    /// <returns>���</returns>
    bool DecomposeTransform(const glm::mat4& transform, glm::vec3& outPosition, glm::vec3& outRotation, glm::vec3& outScale);
}