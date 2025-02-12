/*
 * Copyright (c) 2021-2023, Sam Atkins <atkinssj@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <LibWeb/CSS/Parser/Parser.h>
#include <LibWeb/CSS/Supports.h>

namespace Web::CSS {

Supports::Supports(NonnullOwnPtr<Condition>&& condition)
    : m_condition(move(condition))
{
    m_matches = m_condition->evaluate();
}

bool Supports::Condition::evaluate() const
{
    switch (type) {
    case Type::Not:
        return !children.first().evaluate();
    case Type::And:
        for (auto& child : children) {
            if (!child.evaluate())
                return false;
        }
        return true;
    case Type::Or:
        for (auto& child : children) {
            if (child.evaluate())
                return true;
        }
        return false;
    }
    VERIFY_NOT_REACHED();
}

bool Supports::InParens::evaluate() const
{
    return value.visit(
        [&](NonnullOwnPtr<Condition> const& condition) {
            return condition->evaluate();
        },
        [&](Feature const& feature) {
            return feature.evaluate();
        },
        [&](GeneralEnclosed const&) {
            return false;
        });
}

bool Supports::Declaration::evaluate() const
{
    auto style_property = Parser::Parser({}, declaration).parse_as_supports_condition();
    return style_property.has_value();
}

bool Supports::Selector::evaluate() const
{
    auto style_property = Parser::Parser({}, selector).parse_as_selector();
    return style_property.has_value();
}

bool Supports::Feature::evaluate() const
{
    return value.visit(
        [&](Declaration const& declaration) {
            return declaration.evaluate();
        },
        [&](Selector const& selector) {
            return selector.evaluate();
        });
}

ErrorOr<String> Supports::Declaration::to_string() const
{
    return String::formatted("({})", declaration);
}

ErrorOr<String> Supports::Selector::to_string() const
{
    return String::formatted("selector({})", selector);
}

ErrorOr<String> Supports::Feature::to_string() const
{
    return value.visit([](auto& it) { return it.to_string(); });
}

ErrorOr<String> Supports::InParens::to_string() const
{
    return value.visit(
        [](NonnullOwnPtr<Condition> const& condition) -> ErrorOr<String> { return String::formatted("({})", TRY(condition->to_string())); },
        [](Supports::Feature const& it) -> ErrorOr<String> { return it.to_string(); },
        [](GeneralEnclosed const& it) -> ErrorOr<String> { return it.to_string(); });
}

ErrorOr<String> Supports::Condition::to_string() const
{
    switch (type) {
    case Type::Not:
        return String::formatted("not {}", TRY(children.first().to_string()));
    case Type::And:
        return String::join(" and "sv, children);
    case Type::Or:
        return String::join(" or "sv, children);
    }
    VERIFY_NOT_REACHED();
}

ErrorOr<String> Supports::to_string() const
{
    return m_condition->to_string();
}

}
