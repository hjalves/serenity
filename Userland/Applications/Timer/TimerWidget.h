//
// Created by hjalves on 22/06/22.
//

#pragma once

#include <LibCore/ElapsedTimer.h>
#include <LibGUI/TabWidget.h>
#include <LibGUI/Widget.h>
#include <LibGUI/Window.h>
#include "SplitTimesModel.h"


class TimerWidget : public GUI::Widget {
    C_OBJECT(TimerWidget)

public:
    ~TimerWidget() override = default;
    ErrorOr<void> try_initialize_ui();
    void initialize_menubar(GUI::Window&);

private:
    TimerWidget();
    RefPtr<GUI::Label> m_stopwatch_label;
    RefPtr<GUI::Label> m_split_label;
    RefPtr<GUI::Label> m_split_count_label;

    RefPtr<GUI::Button> m_start_stop_button;
    RefPtr<GUI::Button> m_split_reset_button;

    RefPtr<GUI::Action> m_debug_action;

    RefPtr<GUI::TabWidget> m_tab_widget;

    RefPtr<Gfx::Bitmap> m_start_icon;
    RefPtr<Gfx::Bitmap> m_stop_icon;
    RefPtr<Gfx::Bitmap> m_split_icon;
    RefPtr<Gfx::Bitmap> m_reset_icon;

    RefPtr<SplitTimesModel> m_split_times_model;
    RefPtr<GUI::TableView> m_split_times_table;

    bool m_stopwatch_is_running { false };
    int m_stopwatch_elapsed { 0 };
    int m_stopwatch_split_elapsed { 0 };
    int m_stopwatch_split_counter { 0 };

    Core::ElapsedTimer m_stopwatch_timer { true };
    Core::ElapsedTimer m_stopwatch_split_timer { true };

    void show_start_button();
    void show_stop_button();
    void show_split_button();
    void show_reset_button();

    void stopwatch_start();
    void stopwatch_stop();
    void stopwatch_reset();
    void stopwatch_split();

    int stopwatch_elapsed_total();
    int stopwatch_elapsed_split();

    void update_stopwatch_main_timer();
    void update_stopwatch_split_timer();
    void update_stopwatch_split_count();

    static String format_elapsed(int elapsed_ms);

    virtual void timer_event(Core::TimerEvent&) override;
};
