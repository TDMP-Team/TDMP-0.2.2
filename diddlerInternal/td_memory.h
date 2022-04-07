#pragma once
#include <cstring>

typedef uintptr_t(__fastcall* TMalloc)(size_t);
typedef void(__fastcall* TFree)(uintptr_t mem);

namespace glb {
    extern TMalloc oTMalloc;
    extern TFree oTFree;
}

namespace td {
    class small_string
    {
    public:
        small_string() {};
        small_string(const char* str) {
            size_t len = strlen(str);
            char* dst = m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0];

            if (len > 15)
            {
                dst = (char*)glb::oTMalloc(len + 1);

                if (dst == nullptr)
                {
                    return;
                }

                if (m_StackBuffer[15])
                {
                    glb::oTFree((uintptr_t)m_HeapBuffer);
                }
                else
                {
                    m_StackBuffer[15] = 1;
                }

                m_HeapBuffer = dst;
            }

            memcpy(dst, str, len);
            dst[len] = 0;
        }
        ~small_string() {
            if (m_StackBuffer[15])
            {
                glb::oTFree((uintptr_t)m_HeapBuffer);
            }
        }

        const char* c_str() const { return m_StackBuffer[15] ? m_HeapBuffer : &m_StackBuffer[0]; }

    private:
        union {
            char* m_HeapBuffer;
            char m_StackBuffer[16] = { 0 };
        };
    };
}