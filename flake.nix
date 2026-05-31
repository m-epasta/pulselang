{
  description = "nix-shell for pulselang development";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/25.11";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
  in {
    devShells.${system}.default = pkgs.mkShell {
      buildInputs = with pkgs; [
        # NOTE: Consider setting some programs like your editor into systemPackacges so you can access them while having a *lightweight* nix-shell

        # dev & build tools
        clang-tools
        gcc
        glibc.dev
        tinycc
        gnumake
        bear
        valgrind-light
        # libc

        just
        just-lsp
        just-formatter

        # git utilities
        gh
        git

        # debugging
        gdb
        gdbgui # You may want to just use normal gdb
      ];

      shellHook = ''
        export C_INCLUDE_PATH=${pkgs.glibc.dev}/include
        export CLANGD_FLAGS="--query-driver=${pkgs.gcc}/bin/gcc"
      '';
    };
    packages.${system}.default = pkgs.stdenv.mkDerivation {
      pname = "pulselang";
      version = "0.1.0";
      src = ./.;

      nativeBuildInputs = with pkgs; [gnumake tinycc gcc];

      buildInputs = with pkgs; [glibc.dev];

      buildPhase = ''
        make all
        make embed
        make debug
        make embed_debug
      '';

      installPhase = ''
        mkdir -p $out/bin
        cp build/main $out/bin/
        cp build/tcc/main $out/bin/tmain
        mkdir -p $out/bin/debug
        cp -r build/debug/main $out/bin/debug/main
        cp -r build/tcc/debug $out/bin/debug/tmain
      '';
    };
  };
}
