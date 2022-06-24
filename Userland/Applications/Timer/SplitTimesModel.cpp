/*
 * Copyright (c) 2021, sin-ack <sin-ack@protonmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "SplitTimesModel.h"


String SplitTimesModel::column_name(int column) const
{

    switch (column) {
        case Column::SplitIndex:
            return "Split #";
        case Column::SplitTime:
            return "Split time";
        case Column::TotalTime:
            return "Total time";
        default:
            VERIFY_NOT_REACHED();
    }
}

GUI::Variant SplitTimesModel::data(GUI::ModelIndex const& index, GUI::ModelRole role) const
{
    if (role != GUI::ModelRole::Display)
        return {};
    if (!is_within_range(index))
        return {};

    switch (index.column()) {
        case Column::SplitIndex:
            return index.row() + 1;
        case Column::SplitTime:
            return m_items.at(index.row()).get<0>();
        case Column::TotalTime:
            return m_items.at(index.row()).get<1>();
    }
    VERIFY_NOT_REACHED();
}
//
//TriState SplitTimesModel::data_matches(GUI::ModelIndex const& index, GUI::Variant const& data) const
//{
//    if (!is_within_range(index))
//        return TriState::False;
//    if (!data.is_string())
//        return TriState::False;
//
//    auto& value = m_items.at(index.row());
//    return value.contains(data.as_string()) ? TriState::True : TriState::False;
//}
//
//void SplitTimesModel::invalidate()
//{
//    Model::invalidate();
//    if (on_invalidate)
//        on_invalidate();
//}

GUI::ModelIndex SplitTimesModel::index(int row, int column, GUI::ModelIndex const&) const
{
    if (row < 0 || row >= static_cast<int>(m_items.size()))
        return {};

    return create_index(row, column);
}

//void SplitTimesModel::add_item(String const& item)
//{
//    begin_insert_rows({}, m_items.size(), m_items.size());
//    m_items.append(item);
//    end_insert_rows();
//
//    did_update(UpdateFlag::DontInvalidateIndices);
//}

void SplitTimesModel::add_item(String split_time, String total_time)
{
    begin_insert_rows({}, m_items.size(), m_items.size());
    m_items.append({split_time, total_time});
    end_insert_rows();

    did_update(UpdateFlag::DontInvalidateIndices);
}

void SplitTimesModel::remove_item(GUI::ModelIndex const& index)
{
    if (!index.is_valid() || !is_within_range(index))
        return;

    begin_delete_rows({}, index.row(), index.row());
    m_items.remove(index.row());
    end_delete_rows();

    did_update(UpdateFlag::DontInvalidateIndices);
}
