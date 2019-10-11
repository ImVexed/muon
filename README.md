<p align="center">
<img width="200" height="200" src="./logo.svg" alt="gnet">
<br /> <br />
<a title="Go Report Card" target="_blank" href="https://goreportcard.com/report/github.com/ImVexed/muon"><img src="https://goreportcard.com/badge/github.com/ImVexed/muon?style=flat-square"></a>
<img alt="GitHub code size in bytes" src="https://img.shields.io/github/languages/code-size/ImVexed/muon?style=flat-square">
<br/>
<a target="_blank" href="https://gowalker.org/github.com/ImVexed/muon"><img src="https://img.shields.io/badge/api-reference-blue.svg?style=flat-square"></a>
</p>

----

`Muon` is a lightweight alternative to Electron written in Golang in about ~300 LoC, using Ultralight instead of Chromium. [Ultralight](https://ultralig.ht/) is a cross-platform WebKit rewrite using the GPU to target embedded desktop applications that resulted in a fast, lightweight, and low-memory HTML UI solution that blends the power of Chromium with the small footprint of Native UI.


# Features

- Full JS to Go interop
- GPU based rendering
- Cross-platform
- Hot-reloading
- Superior disk size + memory & cpu usage

Comparison with a "Hello, World!" React App

|      | Muon    | Electron |
|:----:|---------|----------|
| CPU  | 0.0%    | 1.2%     |
| MEM  | 26.0 MB | 201.7 MB |
| DISK | 42 MB   | 136 MB   |

# Example

From `examples/create-react-app/main.go`:
```go
package main

import (
  "github.com/ImVexed/muon"

  "cra-go/webfiles"
  "net/http"
)

func main() {
  // Any static asset packer of your liking (ex. fileb0x)
  fileHandler := http.FileServer(webfiles.HTTP)

  cfg := &muon.Config{
    Title:      "Hello, World!",
    Height:     500,
    Width:      500,
    Titled:     true,
    Resizeable: true,
  }

  m := muon.New(cfg, fileHandler)

  // Expose our `add` function to the JS runtime
  m.Bind("add", add)

  // Show the Window and start the Runtime
  if err := m.Start(); err != nil {
    panic(err)
  }
}

// Muon automatically handles interop to and from the JS runtime
func add(a float64, b float64) float64 {
  return a + b
}
```

# FAQ

## Q: *How are JS types translated to Go types?*
- JS: `Boolean` Go: `bool`
- JS: `Number`  Go: `float64`
- JS: `String`  Go: `string`
- JS: `Object`  Go: `struct` via JSON

## Q: *How do I setup Ultralight?*
- See our [getting started guide](https://github.com/ImVexed/muon/blob/master/getting-started.md)

## Q: *Is there perfect Chrome or Firefox feature parity?*
- No, see [Missing Ultralight Features](https://github.com/ultralight-ux/Ultralight/issues/178)

## Q: *How do I get rid of the Console on Windows?*
- Add `-ldflags -H=windowsgui` to either your `go build` or `go run` to get rid of the window.

## Q: *I get `exit status 3221225781`*
- Your program likely can't find the Ultralight libraries. Ensure they're either installed on the system, or, in the same folder as your program. Currently, Muon uses the 1.1 Ultralight pre-release that hasn't yet propagated to their main site and can only be downloaded from the [Ultralight](https://github.com/ultralight-ux/Ultralight#getting-the-latest-sdk) github repo.

## Q: *I get ` libWebCore.so: cannot open shared object file`*
- If you're attempting to load the libraries out of the same directory as your program add `-ldflags "-r ."` to your `go build` to set `rpath` to the local directory for library resolution.

## Q: *I get `symbol not found: ulCreateSettings`*
- This likely means you're not using the 1.1 Ultralight pre-release which can be downloaded only from their [GitHub Repo](https://github.com/ultralight-ux/Ultralight#getting-the-latest-sdk) for now
# Licensing
While muon itself is MIT licensed, [Ultralight](https://ultralig.ht/) is not. 
```
Ultralight is free for non-commercial use, educational use, 
and also free for commercial use by small indie developers making
less than US$100,000 a year. You can find full terms in the SDK. 
Pricing plans for larger commercial projects will be announced later.
```
Their specific license terms can be found [here](https://github.com/ultralight-ux/Ultralight/tree/master/license).
