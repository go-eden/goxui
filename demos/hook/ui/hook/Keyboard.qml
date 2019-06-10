import QtQuick 2.12

Column {

    property double size: 40

    KeyRow {
        height: 40
        KeyButton {
            label: "Esc"
            code: 27
        }
        KeyButton {
            label: "F1"
            code: 0
        }
        KeyButton {
            label: "F2"
            code: 0
        }
        KeyButton {
            label: "F3"
            code: 0
        }
        KeyButton {
            label: "F4"
            code: 0
        }
        KeyButton {
            label: "F5"
            code: 0
        }
        KeyButton {
            label: "F6"
            code: 0
        }
        KeyButton {
            label: "F7"
            code: 0
        }
        KeyButton {
            label: "F8"
            code: 0
        }
        KeyButton {
            label: "F9"
            code: 0
        }
        KeyButton {
            label: "F10"
            code: 0
        }
        KeyButton {
            label: "F11"
            code: 0
        }
        KeyButton {
            label: "F12"
            code: 0
        }
    }
    KeyRow {
        KeyButton {
            label: "~"
            code: 192
        }
        KeyButton {
            label: "1"
            code: 49
        }
        KeyButton {
            label: "2"
            code: 50
        }
        KeyButton {
            label: "3"
            code: 51
        }
        KeyButton {
            label: "4"
            code: 52
        }
        KeyButton {
            label: "5"
            code: 53
        }
        KeyButton {
            label: "6"
            code: 54
        }
        KeyButton {
            label: "7"
            code: 55
        }
        KeyButton {
            label: "8"
            code: 56
        }
        KeyButton {
            label: "9"
            code: 57
        }
        KeyButton {
            label: "0"
            code: 48
        }
        KeyButton {
            label: "-"
            code: 189
        }
        KeyButton {
            label: "+"
            code: 187
        }
        KeyButtonWide {
            label: "Delete"
            code: 8
            size: 1.5
            align: Text.AlignRight
        }
    }
    KeyRow {
        KeyButtonWide {
            label: "Tab"
            size: 1.4
            code: 9
            align: Text.AlignLeft
        }
        KeyButton {
            label: "Q"
            code: 81
        }
        KeyButton {
            label: "W"
            code: 87
        }
        KeyButton {
            label: "E"
            code: 69
        }
        KeyButton {
            label: "R"
            code: 82
        }
        KeyButton {
            label: "T"
            code: 84
        }
        KeyButton {
            label: "Y"
            code: 89
        }
        KeyButton {
            label: "U"
            code: 85
        }
        KeyButton {
            label: "I"
            code: 73
        }
        KeyButton {
            label: "O"
            code: 79
        }
        KeyButton {
            label: "P"
            code: 80
        }
        KeyButton {
            label: "["
            code: 219
        }
        KeyButton {
            label: "]"
            code: 221
        }
        KeyButton {
            label: "\\"
            code: 220
        }
    }
    KeyRow {
        KeyButtonWide {
            label: "Caps Lock"
            size: 1.6
            code: 20
            align: Text.AlignLeft
        }
        KeyButton {
            label: "A"
            code: 65
        }
        KeyButton {
            label: "S"
            code: 83
        }
        KeyButton {
            label: "D"
            code: 68
        }
        KeyButton {
            label: "F"
            code: 70
        }
        KeyButton {
            label: "G"
            code: 71
        }
        KeyButton {
            label: "H"
            code: 72
        }
        KeyButton {
            label: "J"
            code: 74
        }
        KeyButton {
            label: "K"
            code: 75
        }
        KeyButton {
            label: "L"
            code: 76
        }
        KeyButton {
            label: ";"
            code: 186
        }
        KeyButton {
            label: "'"
            code: 222
        }
        KeyButtonWide {
            label: "Enter"
            size: 1.6
            code: 13
            align: Text.AlignRight
        }
    }
    KeyRow {
        KeyButtonWide {
            label: "Shift"
            size: 2
            code: 16
            align: Text.AlignLeft
        }
        KeyButton {
            label: "Z"
            code: 90
        }
        KeyButton {
            label: "X"
            code: 88
        }
        KeyButton {
            label: "C"
            code: 67
        }
        KeyButton {
            label: "V"
            code: 86
        }
        KeyButton {
            label: "B"
            code: 66
        }
        KeyButton {
            label: "N"
            code: 78
        }
        KeyButton {
            label: "M"
            code: 77
        }
        KeyButton {
            label: ","
            code: 188
        }
        KeyButton {
            label: "."
            code: 190
        }
        KeyButton {
            label: "/"
            code: 191
        }
        KeyButtonWide {
            label: "Shift"
            size: 2
            code: -16
            align: Text.AlignRight
        }
    }
    KeyRow {
        KeyButton{
            label: "Control"
            size: 1.3
            code: 17
        }
        KeyButton{
            label: "Option"
            size: 1.3
            code: 18
        }
        KeyButton{
            label: "Command"
            size: 1.5
            code: 91
        }
        KeyButton{
            label: ""
            size: 5
            code: 32
        }
        KeyButton{
            label: "Command"
            size: 1.5
            code: -93
        }
        KeyButton{
            label: "Option"
            size: 1.3
            code: -18
        }
        KeyButton{
            label: "Control"
            size: 1.3
            code: -17
        }
        KeyButton{
            label: "◀"
            fontSize: 9
            code: 37
        }
        KeyButtonArrow{
        }
        KeyButton{
            label: "▶"
            fontSize: 9
            code: 39
        }
    }
}
