/*
 * Copyright (c) 2020, the SerenityOS developers.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "Example.h"
#include "Canvas.h"
#include <AK/Format.h>
#include <AK/String.h>
#include <AK/Time.h>
#include <AK/URL.h>
#include <LibCore/File.h>
#include <LibCore/System.h>
#include <LibDesktop/Launcher.h>
#include <LibGUI/Application.h>
#include <LibGUI/Icon.h>
#include <LibGUI/Menu.h>
#include <LibGUI/Notification.h>
#include <LibGUI/Window.h>
#include <LibMain/Main.h>
#include <unistd.h>

static RefPtr<GUI::Application> app;

void dbgln(char const* line)
{
    dbgln("{}", line);
}

bool launch(char const* url_or_path)
{
    //    DeprecatedString run_input(url_or_path);
    auto url = URL::create_with_url_or_path(url_or_path);

    if (url.scheme() == "file") {
        auto real_path = Core::File::real_path_for(url.path());
        if (real_path.is_null()) {
            // errno *should* be preserved from Core::File::real_path_for().
            warnln("Failed to launch '{}': {}", url.path(), strerror(errno));
            return false;
        }
        url = URL::create_with_url_or_path(real_path);
    }

    if (!Desktop::Launcher::open(url)) {
        warnln("Failed to launch '{}'", url);
        return false;
    }

    dbgln("Ran via URL launch.");

    return true;
}

void create_app()
{
    Main::Arguments arguments;
    auto result = GUI::Application::try_create(arguments);
    app = result.release_value_but_fixme_should_propagate_errors();
}

void exec_app()
{
    if (app)
        app->exec();
}

void destroy_app()
{
    app.clear();
}

void notify(char const* title, char const* text, char const* icon_path = nullptr)
{
    if (!app)
        return;
    auto notification = GUI::Notification::construct();
    notification->set_title(title);
    notification->set_text(text);
    if (icon_path) {
        auto icon_path_sv = StringView { icon_path, strlen(icon_path) };
        auto icon_bitmap = Gfx::Bitmap::try_load_from_file(icon_path_sv).release_value_but_fixme_should_propagate_errors();
        notification->set_icon(icon_bitmap);
    }
    notification->show();
}

ErrorOr<int> ui_main(RefPtr<GUI::Application> p_app)
{
    if (!p_app)
        return 0;
    auto window = TRY(GUI::Window::try_create());

    window->set_double_buffering_enabled(true);
    window->set_title("LibExample");
    //    window->set_resizable(false);
    window->resize(Canvas::WIDTH, Canvas::HEIGHT);

    auto file_menu = TRY(window->try_add_menu("&File"));
    TRY(file_menu->try_add_action(GUI::CommonActions::make_quit_action([&](auto&) { p_app->quit(); })));

    auto app_icon = TRY(GUI::Icon::try_create_default_icon("app-libgfx-demo"sv));
    window->set_icon(app_icon.bitmap_for_size(16));
    (void)TRY(window->try_set_main_widget<Canvas>());
    window->show();

    return p_app->exec();
}

int mainloop()
{
    auto result = ui_main(app);
    return result.release_value_but_fixme_should_propagate_errors();
}

int fullmainloop()
{
    Main::Arguments arguments;
    auto app_or_error = GUI::Application::try_create(arguments);
    auto appx = app_or_error.release_value_but_fixme_should_propagate_errors();
    //    RefPtr<GUI::Application> appx_ref = appx;
    auto result = ui_main(appx);
    return result.release_value_but_fixme_should_propagate_errors();
}
