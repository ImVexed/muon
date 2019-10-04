# Create-React-App Example

-  Run `yarn && yarn build` inside of `public/`
-  Run `go generate && go build` inside this directory.
-  Ensure Ultralight libraries are installed

## Help! I get `exit status 3221225781` when I try to run the executable!
Due to Ultralight's licensing it can only be dynamically linked to. Meaning you'll need to either add the respective `.dll`'s or `.so`'s to either a system path, or the same directory as the the executable. You can find these libraries on https://ultralig.ht/ (The version that Muon uses can be found in `ultralight/libs/`)