#pragma once

#include <xhash>

namespace Lucky
{
    /// <summary>
    /// 虚拟唯一标识
    /// </summary>
    class UUID
    {
    private:
        uint64_t m_UUID;
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID&) = default;

        operator uint64_t() const { return m_UUID; }
    };
}

namespace std
{
    /// <summary>
    /// UUID 类型哈希
    /// </summary>
    template<>
    struct hash<Lucky::UUID>
    {
        std::size_t operator()(const Lucky::UUID& uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
}