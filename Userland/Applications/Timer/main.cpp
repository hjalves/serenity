/*
 * Copyright (c) 2018-2020, Andreas Kling <kling@serenityos.org>
 * Copyright (c) 2021, Julius Heijmen <julius.heijmen@gmail.com>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "TimerWidget.h"
#include <LibCore/System.h>
#include <LibGUI/AboutDialog.h>
#include <LibGUI/Application.h>
#include <LibGUI/Icon.h>
#include <LibMain/Main.h>

ErrorOr<int> serenity_main(Main::Arguments arguments)
{
    TRY(Core::System::pledge("stdio recvfd sendfd rpath unix"));
    auto app = TRY(GUI::Application::try_create(arguments));

    TRY(Core::System::pledge("stdio recvfd sendfd rpath"));
    TRY(Core::System::unveil("/res", "r"));
    TRY(Core::System::unveil(nullptr, nullptr));

    auto app_icon = TRY(GUI::Icon::try_create_default_icon("app-analog-clock"));
    auto window = TRY(GUI::Window::try_create());
    window->set_title("Timer");
    window->set_icon(app_icon.bitmap_for_size(16));
    window->resize(300, 300);

    auto timer_widget = TRY(window->try_set_main_widget<TimerWidget>());

    TRY(timer_widget->try_initialize_ui());
    timer_widget->initialize_menubar(*window);


    window->show();
    return app->exec();
}

