use cmake::Config;
use std::{env, path::PathBuf};

fn main() {
    let project_dir = PathBuf::from(env::var("CARGO_MANIFEST_DIR").unwrap());
    let cplus_header = project_dir.join("c++/src/lib/interface.h");

    // Build Delphi
    let delphi_install_prefix = Config::new("c++").define("UNITTESTS", "0").build();

    println!(
        "cargo:rustc-link-search={}",
        delphi_install_prefix.display()
    );

    println!(
        "cargo:rustc-link-search={}/lib",
        delphi_install_prefix.display()
    );

    // Link libraries
    println!("cargo:rustc-link-lib=static=seal-3.6");
    println!("cargo:rustc-link-lib=static=DelphiOffline");
    println!("cargo:rustc-link-lib=dylib=stdc++");

    // Tell cargo to invalidate the built crate whenever the wrapper changes
    println!("cargo:rerun-if-changed={}", cplus_header.to_str().unwrap());
    // Run bindgen on c++ directory
    let bindings = bindgen::Builder::default()
        .no_copy("ServerFHE|ClientFHE|ServerTriples|ClientTriples")
        .header(format!("{}", cplus_header.to_str().unwrap()))
        .parse_callbacks(Box::new(bindgen::CargoCallbacks))
        .generate()
        .expect("Unable to generate bindings");

    // Write the bindings to src/bindings.rs
    bindings
        .write_to_file(project_dir.join("src/bindings.rs"))
        .expect("Couldn't write bindings!");
}
