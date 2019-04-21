# goxui

golang &amp; qt bridge

# Qt configuration

By default, Goxui will find Qt library in this location:

- **Darwin**: `/usr/local/opt/qt/lib`
- **Linux**: `/usr/local/qt/lib`
- **Windows**: `C:\`

If your Qt location is different, You should tell compiler via cgo environment:

```python
# for mac, need frameworks
export CGO_LDFLAGS="-F/your/diff/path"
# for linux
export CGO_LDFLAGS="-L/your/diff/path"
# for windows
export CGO_LDFLAGS="-Lc:\your\diff\path"
```