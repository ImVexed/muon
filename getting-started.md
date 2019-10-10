# Getting Started with Muon

While setup varies depending on your operating system, the steps are largely the same no matter what you're on.

Requirements:
 - Go (1.11+)
 - gcc
 - [Ultralight Libraries](https://github.com/ultralight-ux/ultralight#getting-the-latest-sdk) (`\.\bin\` inside the archive)
 
 Start by cloning the repository and entering the example repository:
 
 ```
 git clone https://github.com/ImVexed/muon
 cd muon/examples/create-react-app
 ```
 
Before we can run our project we first need to build and then package all of our frontend assets into a single Go file. There are [many](https://tech.townsourced.com/post/embedding-static-files-in-go/) static asset packagers you can choose from, however we reccomend [fileb0x](https://github.com/UnnoTed/fileb0x) for ease of use and hot reloading.

Start by entering the frontend folder and building it:
```
cd public/
yarn
yarn build
```

Ensure you have [fileb0x](https://github.com/UnnoTed/fileb0x) installed and in your `$PATH` variable by running `go get github.com/UnnoTed/fileb0x` 

Now, back in the `create-react-app` directory, package the assets by running either:
```
fileb0x b0x.yml
```
or
```
go generate
```

Which will tell fileb0x to use [b0x.yml](https://github.com/ImVexed/muon/blob/master/examples/create-react-app/b0x.yml) to pack our `public/build` folder into a go file in `webfiles/`

From there we're good to go as long as we have `gcc` in our path we can run
```
go build
```

Now that we have a binary we're almost done. If you try to run it now, or, if you ran
```
go run main.go
```

You will likely have ran into an error. This is because we are dynamically linking to Ultralight, meaning that it's libraries are resolved when the application is started. To fix this, you'll need the take the [Ultralight Libraries](https://github.com/ultralight-ux/ultralight#getting-the-latest-sdk) (`.dll` for Windows, `.so` for Unix, `.dylib` for Darwin) and put them in the same directory as your application.

**Note for Linux users**
- Linux by default only looks for shared objects at a specific system path, if you would preffer to load the shared objects from a different directory, simply set the `LD_LIBRARY_PATH` environment variable to the new directory containing the libraries. (`.` being current directory)
