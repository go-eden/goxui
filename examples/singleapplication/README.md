# singleapplication

By default, Goxui will prevent the same program run twice, you can disable it like this:

```go
if err := os.Setenv("GOXUI_SINGLE_APPLICATION", "0"); err != nil {
    fmt.Println("setenv error: ", err)
    return
}
```
