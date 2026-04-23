#pragma once

class PathUtil
{
public:
    /// @brief Joins base + name into out with exactly one '/' between them.
    ///        Guarantees null-termination if outSize > 0. If base is empty
    ///        no separator is added.
    /// @return Total bytes written (excluding null), or outSize if truncated.
    static u32 JoinPath(const char* base, const char* name, char* out, u32 outSize);
};
