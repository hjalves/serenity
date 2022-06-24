//
// Created by hjalves on 22/06/22.
//

#include "TimerWidget.h"
#include <Userland/Applications/Timer/StopwatchTabGML.h>
#include <LibGUI/Action.h>
#include <LibGUI/BoxLayout.h>
#include <LibGUI/Button.h>
#include <LibGUI/Label.h>
#include <LibGUI/Menu.h>
#include <LibGUI/TableView.h>
#include <LibGUI/Widget.h>

TimerWidget::TimerWidget()
{
}

ErrorOr<void> TimerWidget::try_initialize_ui()
{
    set_fill_with_background_color(true);
    set_layout<GUI::VerticalBoxLayout>();

    auto icon = TRY(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/app-catdog.png"));
    m_debug_action = GUI::Action::create("Debug", icon,[this](GUI::Action const&) {
        dbgln("This is a debug action!");
        dbgln("{}", m_tab_widget->tab_count());
    });

    m_tab_widget = TRY(try_add<GUI::TabWidget>());

    auto stopwatch_tab = TRY(m_tab_widget->try_add_tab<GUI::Widget>("Stopwatch"));
    stopwatch_tab->load_from_gml(stopwatch_tab_gml);

    // Load buttons
    m_start_icon = TRY(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/play.png"));
    m_stop_icon = TRY(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/pause.png"));
    m_split_icon = TRY(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/plus.png"));
    m_reset_icon = TRY(Gfx::Bitmap::try_load_from_file("/res/icons/16x16/delete.png"));

    // Setup labels
    m_stopwatch_label = stopwatch_tab->find_descendant_of_type_named<GUI::Label>("stopwatch_label");
    m_split_label = stopwatch_tab->find_descendant_of_type_named<GUI::Label>("split_label");
    m_split_count_label = stopwatch_tab->find_descendant_of_type_named<GUI::Label>("split_count_label");

    // Setup buttons
    m_start_stop_button = stopwatch_tab->find_descendant_of_type_named<GUI::Button>("start_stop_button");
    show_start_button();

    m_start_stop_button->on_click = [this](auto modifiers) {
        dbgln("Clicked start/stop button. modifiers {}", modifiers);
        if (!m_stopwatch_is_running)
            stopwatch_start();
        else
            stopwatch_stop();
    };

    m_split_reset_button = stopwatch_tab->find_descendant_of_type_named<GUI::Button>("split_reset_button");
    show_reset_button();

    m_split_reset_button->on_click = [this](auto modifiers) {
        dbgln("Clicked split/reset button. modifiers {}", modifiers);
        if (!m_stopwatch_is_running)
            stopwatch_reset();
        else
            stopwatch_split();
    };

    // Setup table

    m_split_times_model = SplitTimesModel::create();
    m_split_times_table = stopwatch_tab->find_descendant_of_type_named<GUI::TableView>("split_times_table");
    m_split_times_table->set_model(m_split_times_model);

    update_stopwatch_main_timer();
    update_stopwatch_split_timer();
    update_stopwatch_split_count();

    auto timer_tab = TRY(m_tab_widget->try_add_tab<GUI::Widget>("Timer"));
    auto pomodoro_tab = TRY(m_tab_widget->try_add_tab<GUI::Widget>("Pomodoro"));
    auto alarms_tab = TRY(m_tab_widget->try_add_tab<GUI::Widget>("Alarms"));

    return {};
}

void TimerWidget::initialize_menubar(GUI::Window& window)
{
    auto& file_menu = window.add_menu("&File");
    file_menu.add_action(*m_debug_action);

    auto& view_menu = window.add_menu("&View");
    view_menu.add_action(*m_debug_action);

    auto& stopwatch_menu = window.add_menu("&Stopwatch");
    stopwatch_menu.add_action(*m_debug_action);

    auto& help_menu = window.add_menu("&Help");
    help_menu.add_action(*m_debug_action);
}

void TimerWidget::show_start_button()
{
    m_start_stop_button->set_text("Start");
    m_start_stop_button->set_icon(m_start_icon);
}

void TimerWidget::show_stop_button()
{
    m_start_stop_button->set_text("Stop");
    m_start_stop_button->set_icon(m_stop_icon);
}

void TimerWidget::show_split_button()
{
    m_split_reset_button->set_text("Split");
    m_split_reset_button->set_icon(m_split_icon);
}

void TimerWidget::show_reset_button()
{
    m_split_reset_button->set_text("Reset");
    m_split_reset_button->set_icon(m_reset_icon);
}

void TimerWidget::stopwatch_start()
{
    dbgln("Starting stopwatch");
    m_stopwatch_is_running = true;
    show_stop_button();
    show_split_button();
    start_timer(20);
    m_stopwatch_timer.start();
    m_stopwatch_split_timer.start();
}

void TimerWidget::stopwatch_stop()
{
    dbgln("Stopping stopwatch");
    m_stopwatch_is_running = false;

    m_stopwatch_elapsed += m_stopwatch_timer.elapsed();
    m_stopwatch_timer.reset();

    m_stopwatch_split_elapsed += m_stopwatch_split_timer.elapsed();
    m_stopwatch_split_timer.reset();

    show_start_button();
    show_reset_button();
    stop_timer();
    update_stopwatch_main_timer();
    update_stopwatch_split_timer();
}

int TimerWidget::stopwatch_elapsed_total()
{
    int total_ms = m_stopwatch_elapsed;
    if (m_stopwatch_is_running)
        total_ms += m_stopwatch_timer.elapsed();
    return total_ms;
}

int TimerWidget::stopwatch_elapsed_split()
{
    int total_ms = m_stopwatch_split_elapsed;
    if (m_stopwatch_is_running)
        total_ms += m_stopwatch_split_timer.elapsed();
    return total_ms;
}

void TimerWidget::update_stopwatch_main_timer()
{
    int total_ms = stopwatch_elapsed_total();
    auto text = format_elapsed(total_ms);
    m_stopwatch_label->set_text(text);
}

void TimerWidget::update_stopwatch_split_timer()
{
    int split_ms = stopwatch_elapsed_split();
    auto text = format_elapsed(split_ms);
    m_split_label->set_text(text);
}

void TimerWidget::update_stopwatch_split_count()
{
    auto text = String::formatted("Split #{}", m_stopwatch_split_counter + 1);
    m_split_count_label->set_text(text);
}

void TimerWidget::timer_event(Core::TimerEvent&)
{
    update_stopwatch_main_timer();
    update_stopwatch_split_timer();
}

void TimerWidget::stopwatch_reset()
{
    dbgln("Resetting stopwatch");
    if (m_stopwatch_is_running)
        stopwatch_stop();

    m_stopwatch_elapsed = 0;
    m_stopwatch_split_elapsed = 0;
    m_stopwatch_split_counter = 0;

    update_stopwatch_main_timer();
    update_stopwatch_split_timer();
    update_stopwatch_split_count();
}

void TimerWidget::stopwatch_split()
{
    dbgln("Stopwatch new split");
    m_stopwatch_split_counter += 1;

    auto total_elapsed = format_elapsed(stopwatch_elapsed_total());
    auto split_elapsed = format_elapsed(stopwatch_elapsed_split());

    m_split_times_model->add_item(split_elapsed, total_elapsed);

    m_stopwatch_split_elapsed = 0;
    m_stopwatch_split_timer.reset();
    m_stopwatch_split_timer.start();

    update_stopwatch_split_count();
    update_stopwatch_split_timer();
}

String TimerWidget::format_elapsed(int elapsed_ms)
{
    int total_sec = elapsed_ms / 1000;
    int millis = elapsed_ms % 1000;
    int total_min = total_sec / 60;
    int seconds = total_sec % 60;
    int hours = total_min / 60;
    int minutes = total_min % 60;

    StringBuilder builder;
    if (hours >= 1)
        builder.appendff("{:02}:", hours);
    builder.appendff("{:02}:{:02}.{:03}", minutes, seconds, millis);
    builder.trim(1);
    return builder.to_string();
}
