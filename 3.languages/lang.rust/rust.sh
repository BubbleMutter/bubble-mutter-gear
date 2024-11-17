#!/bin/bash

apt install rust-all rust-clippy rust-gdb rustc rustfmt cargo-doc rust-doc

cargo build # build your project
cargo run # run your project
cargo test # test your project
cargo doc # build documentation for your project
cargo publish # publish a library to crates.io

cargo new hello-rust # create project
cargo add ferris-says # add dependence
cargo update  # update libraries to latest version

