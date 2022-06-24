/*
 * Copyright (c) 2021, sin-ack <sin-ack@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Function.h>
#include <AK/NonnullRefPtr.h>
#include <AK/Tuple.h>
#include <AK/Vector.h>
#include <LibGUI/Model.h>

class SplitTimesModel final : public GUI::Model {
public:
    static NonnullRefPtr<SplitTimesModel> create()
    {
        return adopt_ref(*new SplitTimesModel());
    }
    virtual ~SplitTimesModel() override = default;

    enum Column {
        SplitIndex,
        SplitTime,
        TotalTime,
        __Count,
    };

    virtual int row_count(GUI::ModelIndex const& = GUI::ModelIndex()) const override { return m_items.size(); }
    virtual int column_count(GUI::ModelIndex const& = GUI::ModelIndex()) const override { return Column::__Count; }
    virtual String column_name(int) const override;

    virtual GUI::Variant data(GUI::ModelIndex const&, GUI::ModelRole = GUI::ModelRole::Display) const override;
//    virtual TriState data_matches(GUI::ModelIndex const&, GUI::Variant const&) const override;
//    virtual void invalidate() override;
    virtual GUI::ModelIndex index(int row, int column = 0, GUI::ModelIndex const& parent = GUI::ModelIndex()) const override;

    Function<void()> on_invalidate;

    void add_item(String split_time, String total_time);
    void remove_item(GUI::ModelIndex const&);

private:
    SplitTimesModel()
    {
    }

    Vector<Tuple<String, String>> m_items;
};
