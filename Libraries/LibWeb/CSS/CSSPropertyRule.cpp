/*
 * Copyright (c) 2024, Alex Ungurianu <alex@ungurianu.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/Bindings/CSSPropertyRulePrototype.h>
#include <LibWeb/Bindings/Intrinsics.h>
#include <LibWeb/CSS/CSSPropertyRule.h>
#include <LibWeb/CSS/Serialize.h>

namespace Web::CSS {

GC_DEFINE_ALLOCATOR(CSSPropertyRule);

GC::Ref<CSSPropertyRule> CSSPropertyRule::create(JS::Realm& realm, FlyString name, FlyString syntax, bool inherits, RefPtr<CSSStyleValue const> initial_value)
{
    return realm.create<CSSPropertyRule>(realm, move(name), move(syntax), inherits, move(initial_value));
}

CSSPropertyRule::CSSPropertyRule(JS::Realm& realm, FlyString name, FlyString syntax, bool inherits, RefPtr<CSSStyleValue const> initial_value)
    : CSSRule(realm, Type::Property)
    , m_name(move(name))
    , m_syntax(move(syntax))
    , m_inherits(inherits)
    , m_initial_value(move(initial_value))
{
}

Optional<String> CSSPropertyRule::initial_value() const
{
    if (m_initial_value)
        return m_initial_value->to_string(SerializationMode::Normal);
    return {};
}

void CSSPropertyRule::initialize(JS::Realm& realm)
{
    WEB_SET_PROTOTYPE_FOR_INTERFACE(CSSPropertyRule);
    Base::initialize(realm);
}

// https://www.w3.org/TR/cssom-1/#serialize-a-css-rule
String CSSPropertyRule::serialized() const
{
    StringBuilder builder;

    // Serialization algorithm is defined in the spec below
    // https://drafts.css-houdini.org/css-properties-values-api/#the-css-property-rule-interface

    // To serialize a CSSPropertyRule, return the concatenation of the following:

    // 1. The string "@property" followed by a single SPACE (U+0020).
    // 2. The result of performing serialize an identifier on the rule’s name, followed by a single SPACE (U+0020).
    builder.appendff("@property {} ", serialize_an_identifier(name()));

    // 3. The string "{ ", i.e., a single LEFT CURLY BRACKET (U+007B), followed by a SPACE (U+0020).
    builder.append("{ "sv);

    // 4. The string "syntax:", followed by a single SPACE (U+0020).
    // 5. The result of performing serialize a string on the rule’s syntax, followed by a single SEMICOLON (U+003B), followed by a SPACE (U+0020).
    builder.appendff("syntax: {}; ", serialize_a_string(syntax()));

    // 6. The string "inherits:", followed by a single SPACE (U+0020).
    // 7. For the rule’s inherits attribute, one of the following depending on the attribute’s value:
    //      true:  The string "true" followed by a single SEMICOLON (U+003B), followed by a SPACE (U+0020).
    //      false: The string "false" followed by a single SEMICOLON (U+003B), followed by a SPACE (U+0020).
    builder.appendff("inherits: {}; ", inherits());

    // 8. If the rule’s initial-value is present, follow these substeps:
    if (initial_value().has_value()) {
        // 1. The string "initial-value:".
        // 2. The result of performing serialize a CSS value in the rule’s initial-value followed by a single SEMICOLON
        //    (U+003B), followed by a SPACE (U+0020).
        builder.appendff("initial-value: {}; ", initial_value());
    }
    // 9. A single RIGHT CURLY BRACKET (U+007D).
    builder.append("}"sv);

    return MUST(builder.to_string());
}

}
