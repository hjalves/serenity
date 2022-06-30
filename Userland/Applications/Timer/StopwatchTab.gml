@GUI::Widget {
    fill_with_background_color: true
    min_width: 200
    layout: @GUI::VerticalBoxLayout {
        margins: [4]
    }

    @GUI::Widget {
        layout: @GUI::HorizontalBoxLayout {
            margins: [8, 8]
        }
        fixed_height: 90

        @GUI::Frame {
            layout: @GUI::VerticalBoxLayout {
                margins: [8]
            }
            shape: "Panel"
            shadow: "Sunken"
            thickness: 1
            min_width: 150
            max_width: 300

            @GUI::Label {
                name: "stopwatch_label"
                text: "01:23.45"
                font: "Liza"
                font_weight: "Bold"
                font_size: 24
                font: "SevenF"
            }

            @GUI::HorizontalSeparator {}

            @GUI::Widget {
                layout: @GUI::HorizontalBoxLayout {}
                preferred_height: "shrink"

                @GUI::Label {
                    name: "split_count_label"
                    text: "Split #0"
                    text_alignment: "BottomLeft"
                    min_height: 10
                }

                @GUI::Label {
                    name: "split_label"
                    text: "01:23.45"
                    text_alignment: "BottomRight"
                    min_height: 10
                }
            }
        }

        @GUI::VerticalSeparator {}

        @GUI::Widget {
            layout: @GUI::VerticalBoxLayout {}
            max_width: 120

            @GUI::Layout::Spacer {}

            @GUI::Button {
                name: "start_stop_button"
                text: "Start"
            }

            @GUI::Button {
                name: "split_reset_button"
                text: "Split"
                enabled: true
            }

            @GUI::Layout::Spacer {}
        }
    }

    @GUI::GroupBox {
        title: "Split times"
        name: "split_times"
        layout: @GUI::VerticalBoxLayout {
            margins: [5]
        }

        @GUI::TableView {
            name: "split_times_table"
        }
    }
}
