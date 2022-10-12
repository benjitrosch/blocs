#pragma once

namespace blocs
{
    namespace ecs
    {
        template<typename T>
        struct sparse_set
        {
            static_assert(
                std::is_integral<T>::value && std::is_unsigned<T>::value &&
                "ERROR: type of sparse_set must be unsigned integer"
            );

            T* sparse;
            T* dense;

            size max = 0;
            size len = 0;

            sparse_set(size size)
            {
                assert(
                    size > 0 &&
                    "ERROR: cannot have a sparse set size smaller than 1"
                );
                max = size + 1;

                sparse = new T[max];
                dense  = new T[max];

                for (i32 i = 0; i < max; i++)
                {
                    sparse[i] = max - 1U;
                }
            }

            ~sparse_set()
            {
                delete[] sparse;
                delete[] dense;
            }

            const T* begin() { return dense; }
            const T* end() { return &dense[len]; }

            T index(const T& val) const { return sparse[val]; }
            T value(const T& val) const { return dense[sparse[val]]; }

            bool empty() const { return len == 0; }
            void clear()
            {
                len = 0;
                for (i32 i = 0; i < max; i++)
                {
                    sparse[i] = max - 1U;
                }
            }

            bool has(const T& val) const
            {
                return (
                    val < max && sparse[val] < len && sparse[val] != max - 1U
                );
            }

            void add(const T& val)
            {
                if (!has(val))
                {
                    assert(
                        val >= 0 && val < max &&
                        "ERROR: entity id exceeded sparse set range"
                    );

                    dense[len]  = val;
                    sparse[val] = len;

                    ++len;
                }
            }

            void remove(const T& val)
            {
                if (has(val))
                {
                    dense[sparse[val]]     = dense[len - 1];
                    sparse[dense[len - 1]] = sparse[val];

                    sparse[val] = max - 1U;

                    --len;
                }
            }
        };
    }
}
