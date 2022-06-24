serenity_component(
    Timer
    TARGETS Timer
)

compile_gml(StopwatchTab.gml StopwatchTabGML.h stopwatch_tab_gml)

set(SOURCES
    main.cpp
    SplitTimesModel.cpp
    StopwatchTabGML.h
    TimerWidget.cpp
)

serenity_app(Timer ICON app-analog-clock)
target_link_libraries(Timer LibGUI LibCore LibMain)
