/**
 *  \file google_protobuf.hpp
 *
 *  Copyright 2015 Michal Bukovsky
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_BOOSTACHE_MODEL_EXTENSIONS_GOOGLE_PROTOBUF_HPP
#define BOOST_BOOSTACHE_MODEL_EXTENSIONS_GOOGLE_PROTOBUF_HPP

#include <type_traits>
#include <boost/boostache/model/category.hpp>
#include <google/protobuf/message.h>

namespace boost { namespace boostache { namespace extension {
namespace gpe {

template <typename Result>
Result fake_return() { return Result{};}

template <>
void fake_return<void>() {}

template <typename Result = void, typename Callback = void>
Result call_if_exist(const google::protobuf::Message &context,
                     const std::string &tag,
                     Callback callback)
{
    if (auto refl = context.GetReflection()) {
        if (auto desc = context.GetDescriptor()) {
            if (auto field = desc->FindFieldByName(tag)) {
                return callback(refl, field);
            }
        }
    }
    return fake_return<Result>();
}

template <typename Result = void, typename Callback = void>
Result dispatch(const google::protobuf::Message &context,
                const google::protobuf::Reflection *refl,
                const google::protobuf::FieldDescriptor *field,
                Callback callback)
{
    switch (field->type()) {
    case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
        return callback(refl->GetDouble(context, field));
    case google::protobuf::FieldDescriptor::TYPE_FLOAT:
        return callback(refl->GetFloat(context, field));
    case google::protobuf::FieldDescriptor::TYPE_FIXED64:
    case google::protobuf::FieldDescriptor::TYPE_SFIXED64:
    case google::protobuf::FieldDescriptor::TYPE_SINT64:
    case google::protobuf::FieldDescriptor::TYPE_INT64:
        return callback(refl->GetInt64(context, field));
    case google::protobuf::FieldDescriptor::TYPE_FIXED32:
    case google::protobuf::FieldDescriptor::TYPE_SFIXED32:
    case google::protobuf::FieldDescriptor::TYPE_SINT32:
    case google::protobuf::FieldDescriptor::TYPE_INT32:
        return callback(refl->GetInt32(context, field));
    case google::protobuf::FieldDescriptor::TYPE_UINT64:
        return callback(refl->GetUInt64(context, field));
    case google::protobuf::FieldDescriptor::TYPE_UINT32:
        return callback(refl->GetUInt32(context, field));
    case google::protobuf::FieldDescriptor::TYPE_BOOL:
        return callback(refl->GetBool(context, field));
    case google::protobuf::FieldDescriptor::TYPE_STRING:
        return callback(refl->GetString(context, field));
    case google::protobuf::FieldDescriptor::TYPE_BYTES:
        return callback(refl->GetString(context, field));
    case google::protobuf::FieldDescriptor::TYPE_ENUM:
        return callback(refl->GetEnum(context, field)->name());
    case google::protobuf::FieldDescriptor::TYPE_GROUP:
    case google::protobuf::FieldDescriptor::TYPE_MESSAGE:
        return callback(refl->GetMessage(context, field));
    }
    throw std::runtime_error("unknown protobuf entry type");
}

template <typename>
struct is_message: public std::false_type {};

template <>
struct is_message<google::protobuf::Message>: public std::true_type {};

} // namespace gpe

struct test_call {
    template <
        typename Value,
        typename std::enable_if<gpe::is_message<Value>::value, int>::type = 0
    >
    bool operator()(const Value &) const { return true;}

    template <
        typename Value,
        typename std::enable_if<!gpe::is_message<Value>::value, int>::type = 0
    >
    bool operator()(const Value &value) const {
        return test(value, typename test_category<decltype(value)>::type{});
    }
};

template <typename Stream>
struct render_call {
    render_call(Stream &&stream, const std::string &tag)
        : stream(std::forward<Stream>(stream)), tag(tag)
    {}

    template <
        typename Value,
        typename std::enable_if<gpe::is_message<Value>::value, int>::type = 0
    >
    void operator()(const Value &) const {}

    template <
        typename Value,
        typename std::enable_if<!gpe::is_message<Value>::value, int>::type = 0
    >
    void operator()(const Value &value) const {
        std::cout << "RENDER_FOUND: '" << tag << "'='" << value << std::endl;
        render(std::forward<Stream>(stream), value, tag);
    }

    Stream &&stream;
    const std::string &tag;
};

bool test(const google::protobuf::Message &context,
          const std::string &tag,
          plain_attribute)
{
    std::cout << "TEST: '" << tag << "'" << std::endl;
    return gpe::call_if_exist<bool>(context, tag,
            [&] (const google::protobuf::Reflection *refl,
                 const google::protobuf::FieldDescriptor *field)
    {
        if (field->is_repeated()) return bool(refl->FieldSize(context, field));
        return gpe::dispatch<bool>(context, refl, field, test_call{});
    });
}

template <typename Stream>
void render(Stream &&stream,
            const google::protobuf::Message &context,
            const std::string &tag,
            plain_attribute)
{
    std::cout << "RENDER: '" << tag << "'" << std::endl;
    gpe::call_if_exist<>(context, tag,
            [&] (const google::protobuf::Reflection *refl,
                 const google::protobuf::FieldDescriptor *field)
    {
        if (field->is_repeated()) return;
        gpe::dispatch<>(context, refl, field, render_call<Stream>{stream, tag});
    });
}

template <typename Stream>
void select_context_dispatch(Stream &stream,
                             const vm::ast::select_context &v,
                             const google::protobuf::Message &context,
                             plain_attribute)
{
    std::cout << "SELECT: '" << v.tag << "' " << std::endl;
    gpe::call_if_exist<>(context, v.tag,
            [&] (const google::protobuf::Reflection *refl,
                 const google::protobuf::FieldDescriptor *field)
    {
        if (field->type() != field->TYPE_MESSAGE) return;
        if (field->is_repeated()) {
            using google::protobuf::Message;
            auto &repeat = refl->GetRepeatedPtrField<Message>(context, field);
            generate(stream, v.body, repeat);
        } else generate(stream, v.body, refl->GetMessage(context, field));
    });
}

template <typename Stream, typename Context>
struct generate_dispatcher<
    Stream,
    Context,
    typename std::enable_if<
        std::is_base_of<google::protobuf::Message, Context>::value
    >::type
> {
    void operator()(Stream &stream,
                    const vm::ast::node &templ,
                    const Context &context) const
    {
        auto &parent = static_cast<const google::protobuf::Message &>(context);
        vm::generate(stream, templ, parent);
    }
};

}}} // namespace boost::boostache::extension

#endif /* BOOST_BOOSTACHE_MODEL_EXTENSIONS_GOOGLE_PROTOBUF_HPP */

