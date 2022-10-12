#pragma once

#define NUM_ARGS_(Zero, I1, I2, I3, I4, I5, I6, I7, I8, A, ...) A
#define NUM_ARGS(...)                                           NUM_ARGS_(0, __VA_ARGS__, 08, 07, 06, 05, 04, 03, 02, 01)

#define TYPE_NAME_(NAME)                                        #NAME
#define TYPE_NAME(NAME)                                         TYPE_NAME_(NAME)

#define FIELD_NAME_(NAME)                                       #NAME
#define FIELD_NAME(NAME)                                        FIELD_NAME_(NAME)

#define CALL_ACTION_(Result)                                    Result
#define CALL_ACTION(Act, Type, Field)                           CALL_ACTION_(Act(Type, Field))

#define REP_N(Act, P1, ...) \
    REP_OF_N(Act, P1, NUM_ARGS(__VA_ARGS__), __VA_ARGS__)
#define REP_OF_N(Act, P1, Count, ...)  REP_OF_N_(Act, P1, Count, __VA_ARGS__)
#define REP_OF_N_(Act, P1, Count, ...) REP_OF_##Count(Act, P1, __VA_ARGS__)

#define REP_OF_08(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_07(Act, P1, __VA_ARGS__)
#define REP_OF_07(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_06(Act, P1, __VA_ARGS__)
#define REP_OF_06(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_05(Act, P1, __VA_ARGS__)
#define REP_OF_05(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_04(Act, P1, __VA_ARGS__)
#define REP_OF_04(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_03(Act, P1, __VA_ARGS__)
#define REP_OF_03(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_02(Act, P1, __VA_ARGS__)
#define REP_OF_02(Act, P1, P2, ...) \
    CALL_ACTION(Act, P1, P2), REP_OF_01(Act, P1, __VA_ARGS__)
#define REP_OF_01(Act, P1, P2) CALL_ACTION(Act, P1, P2)

#define BIND_TYPE(Type, Field) std::pair<char const*, decltype(&Type::Field)>
#define BIND_VALUE(Type, Field)         \
    {                                   \
        FIELD_NAME(Field), &Type::Field \
    }

#define SERIALIZE(DataType, ...)                                         \
    namespace blocs                                                      \
    {                                                                    \
        namespace serializer                                             \
        {                                                                \
            template<>                                                   \
            struct Blueprint<DataType>                                   \
            {                                                            \
                using Fields =                                           \
                    std::tuple<REP_N(BIND_TYPE, DataType, __VA_ARGS__)>; \
                                                                         \
                static str const& get_type_name()                        \
                {                                                        \
                    static str type_name = TYPE_NAME(DataType);          \
                    return type_name;                                    \
                }                                                        \
                                                                         \
                static Fields const& get_fields()                        \
                {                                                        \
                    static constexpr Fields fields{                      \
                        REP_N(BIND_VALUE, DataType, __VA_ARGS__)};       \
                    return fields;                                       \
                }                                                        \
            };                                                           \
        }                                                                \
    }

#include <string_view>

#include <blocs/ecs/world.h>

namespace blocs
{
    namespace serializer
    {
        template<typename T>
        constexpr auto type_name()
        {
            std::string_view name, prefix, suffix;
#ifdef __clang__
            name   = __PRETTY_FUNCTION__;
            prefix = "auto blocs::serializer::type_name() [T = const ";
            suffix = " &]";
#elif defined(__GNUC__)
            name = __PRETTY_FUNCTION__;
            prefix =
                "constexpr auto blocs::serializer::type_name() [with T = "
                "const ";
            suffix = " &]";
#elif defined(_MSC_VER)
            name   = __FUNCSIG__;
            prefix = "auto __cdecl blocs::serializer::type_name<const ";
            suffix = " &>(void)";
#endif
            name.remove_prefix(prefix.size());
            name.remove_suffix(suffix.size());
            return name;
        }

        template<typename T>
        struct Blueprint
        {
        };

#ifdef DEBUG
        namespace json
        {
            namespace
            {
                inline void open_object(ss& stream) { stream << '{'; }

                inline void close_object(ss& stream) { stream << '}'; }

                inline void open_array(ss& stream) { stream << '['; }

                inline void close_array(ss& stream) { stream << ']'; }

                template<typename T>
                inline void add_key(ss& stream, T name)
                {
                    stream << '"';
                    stream << name;
                    stream << '"';
                }

                template<typename T, typename Fields>
                inline void serialize_object_to_json(
                    ss& stream, T const& object, Fields const& fields
                );

                template<typename T, typename Fields>
                inline void serialize_objects_to_json(
                    ss& stream, std::vector<T> objects, Fields const& fields
                );

                template<typename TKey, typename TValue, typename Fields>
                inline void serialize_map_to_json(
                    ss& stream, std::unordered_map<TKey, TValue> map,
                    Fields const& fields
                );

                inline void add_value(ss& stream, bool val)
                {
                    stream << std::boolalpha << val << std::noboolalpha;
                }

                inline void add_value(ss& stream, std::nullptr_t)
                {
                    stream << "null";
                }

                inline void add_value(ss& stream, cstr val)
                {
                    stream << '"' << val << '"';
                }

                inline void add_value(ss& stream, str val)
                {
                    stream << '"' << val << '"';
                }

                template<typename T>
                inline void add_value(ss& stream, T const& val)
                {
                    if constexpr (std::is_fundamental<T>::value)
                        stream << val;
                    else
                        serialize_object_to_json(
                            stream, val, Blueprint<T>::get_fields()
                        );
                }

                template<typename T>
                inline void add_value(ss& stream, const std::vector<T>& values)
                {
                    if constexpr (std::is_fundamental<T>::value)
                    {
                        open_array(stream);

                        if (values.size())
                        {
                            for (auto const& val : values)
                            {
                                add_value(stream, val);
                                stream << ',';
                            }

                            stream.seekp(-1, stream.cur);
                        }

                        close_array(stream);
                    }
                    else
                    {
                        open_object(stream);
                        serialize_objects_to_json(
                            stream, values, Blueprint<T>::get_fields()
                        );
                        close_object(stream);
                    }
                }

                template<typename TKey, typename TValue>
                inline void add_value(
                    ss& stream, const std::unordered_map<TKey, TValue>& map
                )
                {
                    if constexpr (std::is_fundamental<TValue>::value)
                    {
                        open_object(stream);

                        if (map.size())
                        {
                            for (auto const& [name, val] : map)
                            {
                                add_key(stream, name);
                                stream << ':';

                                add_value(stream, val);
                                stream << ',';
                            }

                            stream.seekp(-1, stream.cur);
                        }

                        close_object(stream);
                    }
                    else
                    {
                        open_object(stream);
                        serialize_map_to_json(
                            stream, map, Blueprint<TValue>::get_fields()
                        );
                        close_object(stream);
                    }
                }

                template<typename T>
                inline void add_field(
                    ss& stream, str const& key, T const& value
                )
                {
                    add_key(stream, key);
                    stream << ':';
                    add_value(stream, value);
                    stream << ',';
                }

                template<typename T, typename M>
                inline void add_field(
                    ss& stream, T const& object, M const& field
                )
                {
                    add_key(stream, field.first);
                    stream << ':';
                    add_value(stream, object.*(field.second));
                    stream << ',';
                }

                template<typename T, typename M>
                inline void add_type(
                    ss& stream, T const& object, M const& field
                )
                {
                    add_key(stream, field.first);
                    stream << ':';
                    add_value(
                        stream,
                        (str)type_name<decltype(object.*(field.second))>()
                    );
                    stream << ',';
                }

                template<typename T, typename Fields, std::size_t... Seq>
                inline void
                construct_json_body(ss& stream, T const& object, Fields const& fields, std::index_sequence<Seq...> const&)
                {
                    ((add_field(stream, object, std::get<Seq>(fields))), ...);
                }

                template<typename T, typename Fields, std::size_t... Seq>
                inline void
                get_types(ss& stream, T const& object, Fields const& fields, std::index_sequence<Seq...> const&)
                {
                    ((add_type(stream, object, std::get<Seq>(fields))), ...);
                }

                template<typename T, typename Fields>
                inline void serialize_object_to_json(
                    ss& stream, T const& object, Fields const& fields
                )
                {
                    open_object(stream);

                    construct_json_body(
                        stream, object, fields,
                        std::make_index_sequence<
                            std::tuple_size<Fields>::value>()
                    );

                    stream.seekp(-1, stream.cur);
                    close_object(stream);
                }

                template<typename T, typename Fields>
                inline void serialize_objects_to_json(
                    ss& stream, std::vector<T> objects, Fields const& fields
                )
                {
                    add_key(stream, Blueprint<T>::get_type_name());
                    stream << ':';
                    open_object(stream);

                    add_key(stream, "data");
                    stream << ':';
                    open_array(stream);

                    for (auto const& object : objects)
                    {
                        open_object(stream);

                        construct_json_body(
                            stream, object, fields,
                            std::make_index_sequence<
                                std::tuple_size<Fields>::value>()
                        );
                        stream.seekp(-1, stream.cur);

                        close_object(stream);
                        stream << ',';
                    }

                    if (objects.size()) stream.seekp(-1, stream.cur);

                    close_array(stream);
                    stream << ',';

                    add_key(stream, "types");
                    stream << ':';
                    open_object(stream);

                    get_types(
                        stream, T{}, fields,
                        std::make_index_sequence<
                            std::tuple_size<Fields>::value>()
                    );
                    stream.seekp(-1, stream.cur);

                    close_object(stream);

                    close_object(stream);
                }

                template<typename TKey, typename TValue, typename Fields>
                inline void serialize_map_to_json(
                    ss& stream, std::unordered_map<TKey, TValue> map,
                    Fields const& fields
                )
                {
                    const TKey& empty_key_type = {};
                    str         key_type_name =
                        str(type_name<decltype(empty_key_type)>());

                    add_key(
                        stream, str("{ ") + key_type_name + ": " +
                                    Blueprint<TValue>::get_type_name() + " }"
                    );
                    stream << ':';
                    open_object(stream);

                    add_key(stream, "data");
                    stream << ':';
                    open_object(stream);

                    for (auto const& [name, object] : map)
                    {
                        add_key(stream, name);
                        stream << ':';

                        open_object(stream);
                        construct_json_body(
                            stream, object, fields,
                            std::make_index_sequence<
                                std::tuple_size<Fields>::value>()
                        );
                        stream.seekp(-1, stream.cur);
                        close_object(stream);
                        stream << ',';
                    }

                    if (map.size()) stream.seekp(-1, stream.cur);
                    close_object(stream);
                    stream << ',';

                    add_key(stream, "types");
                    stream << ':';
                    open_object(stream);

                    add_key(stream, "key");
                    stream << ':';
                    add_key(stream, key_type_name);
                    stream << ',';

                    add_key(stream, "value");
                    stream << ':';
                    open_object(stream);

                    get_types(
                        stream, TValue{}, fields,
                        std::make_index_sequence<
                            std::tuple_size<Fields>::value>()
                    );
                    stream.seekp(-1, stream.cur);
                    close_object(stream);

                    close_object(stream);

                    close_object(stream);
                }

                template<typename T, typename Fields>
                inline void serialize_components_to_json(
                    ss& stream, ecs::World::QueryResult<T> objects,
                    Fields const& fields
                )
                {
                    add_key(stream, Blueprint<T>::get_type_name());
                    stream << ':';
                    open_object(stream);

                    add_key(stream, "data");
                    stream << ':';
                    open_array(stream);

                    for (auto const& [entity, object] : objects)
                    {
                        open_object(stream);

                        add_field<i64>(stream, "entity_id", entity);

                        add_key(stream, "component");
                        stream << ':';

                        open_object(stream);
                        construct_json_body(
                            stream, object, fields,
                            std::make_index_sequence<
                                std::tuple_size<Fields>::value>()
                        );
                        stream.seekp(-1, stream.cur);
                        close_object(stream);

                        close_object(stream);
                        stream << ',';
                    }

                    if (objects.size()) stream.seekp(-1, stream.cur);

                    close_array(stream);
                    stream << ',';

                    add_key(stream, "types");
                    stream << ':';
                    open_object(stream);

                    get_types(
                        stream, T{}, fields,
                        std::make_index_sequence<
                            std::tuple_size<Fields>::value>()
                    );
                    stream.seekp(-1, stream.cur);

                    close_object(stream);

                    close_object(stream);
                    stream << ',';
                }

                inline void serialize_names_to_json(
                    ss& stream, std::unordered_map<ecs::Entity, str>& names
                )
                {
                    open_object(stream);

                    for (auto const& [entity, name] : names)
                    {
                        add_field<str>(stream, std::to_string(entity), name);
                    }

                    if (names.size()) stream.seekp(-1, stream.cur);
                    close_object(stream);
                }

                template<typename T, typename Fields>
                inline void serialize_resource_to_json(
                    ss& stream, T const& object, Fields const& fields
                )
                {
                    add_key(stream, Blueprint<T>::get_type_name());
                    stream << ':';

                    open_object(stream);

                    add_key(stream, "data");
                    stream << ':';
                    open_object(stream);

                    construct_json_body(
                        stream, object, fields,
                        std::make_index_sequence<
                            std::tuple_size<Fields>::value>()
                    );

                    stream.seekp(-1, stream.cur);
                    close_object(stream);
                    stream << ',';

                    add_key(stream, "types");
                    stream << ':';
                    open_object(stream);

                    get_types(
                        stream, object, fields,
                        std::make_index_sequence<
                            std::tuple_size<Fields>::value>()
                    );
                    stream.seekp(-1, stream.cur);

                    close_object(stream);

                    close_object(stream);
                    stream << ',';
                }
            }

            template<typename T>
            inline str to_json(T const& object)
            {
                ss stream;
                serialize_object_to_json(
                    stream, object, Blueprint<T>::get_fields()
                );
                return stream.str();
            }

            template<typename... Types>
            inline str to_json(std::vector<Types>&... objects)
            {
                ss stream;

                ((serialize_objects_to_json(
                     stream, objects, Blueprint<Types>::get_fields()
                 )),
                 ...);

                return stream.str();
            }

            template<typename TKey, typename... Types>
            inline str to_json(std::unordered_map<TKey, Types>&... maps)
            {
                ss stream;

                open_object(stream);

                ((serialize_map_to_json(
                     stream, maps, Blueprint<Types>::get_fields()
                 )),
                 ...);

                close_object(stream);

                return stream.str();
            }

            inline str names_to_json(std::unordered_map<ecs::Entity, str>& names
            )
            {
                ss stream;

                serialize_names_to_json(stream, names);

                return stream.str();
            }

            template<typename... Types>
            inline str components_to_json(
                ecs::World::QueryResult<Types>... components
            )
            {
                ss stream;

                open_object(stream);

                ((serialize_components_to_json(
                     stream, components, Blueprint<Types>::get_fields()
                 )),
                 ...);

                constexpr i64 size = sizeof...(Types);
                if (size > 0)
                {
                    stream.seekp(-1, stream.cur);
                }
                close_object(stream);

                return stream.str();
            }

            template<typename... Types>
            inline str resources_to_json(const Types&... resources)
            {
                ss stream;

                open_object(stream);

                ((serialize_resource_to_json(
                     stream, resources, Blueprint<Types>::get_fields()
                 )),
                 ...);

                constexpr i64 size = sizeof...(Types);
                if (size > 0)
                {
                    stream.seekp(-1, stream.cur);
                }
                close_object(stream);

                return stream.str();
            }
        }
#endif
    }
}
