# singleapplication

By default, Goxui will prevent the same program run twice, you can disable it like this:

```go
if err := os.Setenv("GOXUI_SINGLE_APPLICATION", "0"); err != nil {
    fmt.Println("setenv error: ", err)
    return
}
```

When the second process was started, it will exit directly, and trigger the first process's `app_active` event, 
you can accept it in qml like this:

```qml
import QtQuick 2.9
import QtQuick.Controls 2.2
import Goxui 1.0

Window {
    id: mainWindow

    Event {
        key: "app_active"
        onActive: {
            mainWindow.raise()
            mainWindow.show()
        }
    }

}
```