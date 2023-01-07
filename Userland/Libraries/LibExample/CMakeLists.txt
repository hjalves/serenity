serenity_component(
    LibExample
    TARGETS LibExample
)

set(SOURCES
    Example.cpp
    Canvas.cpp
)

serenity_lib(LibExample example)
target_link_libraries(LibExample PRIVATE LibCore LibDesktop LibGUI LibGfx)
